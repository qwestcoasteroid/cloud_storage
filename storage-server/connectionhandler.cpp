#include "connectionhandler.h"
#include "protocol.h"
#include "databaseconnection.h"
#include <QUuid>
#include <QCoreApplication>
#include <QHostAddress>

QByteArray getFileHeader(const QString &uuid) {
    QByteArray result;
    QFile file(uuid);
    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_5_0);
    file.open(QIODevice::ReadOnly);

    stream >> result;

    file.close();

    return result;
}

bool validateToken(const QString &token, const QHostAddress &address) {
    auto info = SessionsManager::instance()->getInfo(token);

    return info != nullptr && info->getAddress().toString() == address.toString();
}

void ConnectionHandler::handleDeletion(SessionInfo *session, const QByteArray &data) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_0);

    QString uuid;

    stream >> uuid;

    auto writer = connection->getWriter();

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    auto userId = session->getProfile().id;

    if (!DataBaseConnection::instance()->fileExists(userId, uuid)) {
        responseStream << QString("File doesn't exist!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    QFileInfo info(uuid);

    DataBaseConnection::instance()->deleteFile(uuid);
    DataBaseConnection::instance()->updateStatistics(userId, -info.size());

    QFile::remove(uuid);

    writer->write(MessageID::DeleteFileRequest);

    isHandled = true;
}

void ConnectionHandler::handleProfile(SessionInfo *session, const QByteArray &data) {
    Q_UNUSED(data)

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    auto writer = connection->getWriter();

    auto userInfo = DataBaseConnection::instance()->getUserInfo(session->getProfile().login);

    if (!userInfo) {
        responseStream << QString("Profile info not found!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    responseStream << userInfo->login << userInfo->freeSpace << userInfo->files;

    writer->write(MessageID::ProfileRequest, &response);

    isHandled = true;
}

void ConnectionHandler::handleRegistration(const QByteArray &data) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_0);

    QString login, tariff;
    QByteArray password;

    stream >> login >> password >> tariff;

    auto writer = connection->getWriter();

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    if (DataBaseConnection::instance()->userExists(login)) {
        responseStream << QString("User already exists!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    auto tariffId = DataBaseConnection::instance()->getTariffId(tariff);

    if (!tariffId.has_value()) {
        responseStream << QString("Internal error!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    if (!DataBaseConnection::instance()->addUser(login, password, tariffId.value())) {
        responseStream << QString("Internal error!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    writer->write(MessageID::RegistrationRequest);

    isHandled = true;
}

void ConnectionHandler::handleAuthorization(const QByteArray &data) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_0);

    QString login;
    QByteArray password;

    stream >> login >> password;

    auto writer = connection->getWriter();

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    if (!DataBaseConnection::instance()->validateUser(login, password)) {
        responseStream << QString("Incorrect login or password!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    auto profileInfo = DataBaseConnection::instance()->getProfileInfo(login);

    if (!profileInfo) {
        responseStream << QString("Sorry, your profile don't found!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    auto oldToken = SessionsManager::instance()->getToken(*profileInfo, connection->getSocket().peerAddress());

    if (oldToken.size() != 0) {
        responseStream << oldToken;
    }
    else {
        responseStream << SessionsManager::instance()->addSession(*profileInfo, connection->getSocket().peerAddress());
    }

    writer->write(MessageID::AuthorizationRequest, &response);

    isHandled = true;
}

void ConnectionHandler::handleStorageContent(SessionInfo *session, const QByteArray &data) {
    Q_UNUSED(data)

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    auto writer = connection->getWriter();

    auto files = DataBaseConnection::instance()->getFiles(session->getProfile().id);

    for (const auto &item : files) {
        QByteArray header = getFileHeader(item);
        responseStream << header << item;
    }

    writer->write(MessageID::StorageContentRequest, &response);

    isHandled = true;
}

void ConnectionHandler::handleUpload(SessionInfo *session, const QByteArray &data) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_0);

    QByteArray header;
    quint64 fileSize;

    stream >> header >> fileSize;

    auto userInfo = DataBaseConnection::instance()->getUserInfo(session->getProfile().login);

    auto writer = connection->getWriter();

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    if (userInfo->freeSpace < fileSize) {
        responseStream << QString("Not enough space!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    QString uuid;

    do {
        uuid = QUuid::createUuid().toString();
    } while (QFile::exists(uuid));

    transfer.setFileName(uuid);
    transfer.open(QIODevice::WriteOnly);

    fileStream.setDevice(&transfer);
    fileStream.setVersion(QDataStream::Qt_5_0);

    fileStream << header;

    writer->write(MessageID::FileChunkRequest);
}

void ConnectionHandler::receiveFileChunk(const QByteArray &data) {
    fileStream << data;
    auto writer = connection->getWriter();
    writer->write(MessageID::FileChunkRequest);
}

void ConnectionHandler::handleEndOfFile(SessionInfo *session) {
    if (transfer.openMode().testFlag(QIODevice::WriteOnly)) {
        DataBaseConnection::instance()->addFile(session->getProfile().id,
                                                QFileInfo(transfer.fileName()).fileName());
        DataBaseConnection::instance()->updateStatistics(session->getProfile().id,
                                                         transfer.size());

        auto writer = connection->getWriter();

        writer->write(MessageID::EndOfFile);
    }

    transfer.close();

    isHandled = true;
}

void ConnectionHandler::handleDownload(SessionInfo *session, const QByteArray &data) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_0);

    QString uuid;

    stream >> uuid;

    auto writer = connection->getWriter();

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    auto userId = session->getProfile().id;

    if (!DataBaseConnection::instance()->fileExists(userId, uuid)) {
        responseStream << QString("File doesn't exist!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    transfer.setFileName(uuid);
    transfer.open(QIODevice::ReadOnly);

    fileStream.setDevice(&transfer);
    fileStream.setVersion(QDataStream::Qt_5_0);

    fileStream >> response;

    writer->write(MessageID::DownloadFileResponse);
}

void ConnectionHandler::sendFileChunk() {
    auto writer = connection->getWriter();

    if (!fileStream.atEnd()) {
        QByteArray fileChunk;
        fileStream >> fileChunk;
        writer->write(MessageID::FileChunkResponse, &fileChunk);
    }
    else {
        writer->write(MessageID::EndOfFile);
    }
}

void ConnectionHandler::handleBadToken() {
    auto writer = connection->getWriter();

    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);

    responseStream << QString("Invalid access token!");

    writer->write(MessageID::Error, &response);

    isHandled = true;
}

void ConnectionHandler::handleTariffs() {
    QByteArray response;
    QDataStream responseStream(&response, QIODevice::WriteOnly);
    responseStream.setVersion(QDataStream::Qt_5_0);
    auto result = DataBaseConnection::instance()->getTariffs();
    auto writer = connection->getWriter();

    if (result.empty()) {
        responseStream << QString("Invalid access token!");
        writer->write(MessageID::Error, &response);
        isHandled = true;
        return;
    }

    for (int i = 0; i < result.size(); ++i) {
        responseStream << result[i].name << result[i].space;
    }

    writer->write(MessageID::TariffsRequest, &response);

    isHandled = true;
}

ConnectionHandler::ConnectionHandler(quintptr socketDescriptor, QObject *parent) :
    QObject(parent), socketDescriptor(socketDescriptor),
    connection(new Connection(socketDescriptor))  {

    connect(connection->getReader(), &NetworkReader::packetReady, this, &ConnectionHandler::onPacketReady);
    connect(connection, &Connection::connectionClosed, this, &ConnectionHandler::onClosed);
    connect(connection, &Connection::connectionClosed, connection, &Connection::deleteLater);
}

void ConnectionHandler::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    QString loginString{ connection->getSocket().peerAddress().toString() };

    bool tokenIsGood{};
    MessageID id = newPacket->header.id;

    switch (id) {
    case MessageID::RegistrationRequest:
        qDebug() << QString("Processing %1 [%2]").arg(messageStrings[id]).arg(loginString);
        handleRegistration(newPacket->data);
        break;
    case MessageID::AuthorizationRequest:
        qDebug() << QString("Processing %1 [%2]").arg(messageStrings[id]).arg(loginString);
        handleAuthorization(newPacket->data);
        break;
    case MessageID::TariffsRequest:
        qDebug() << QString("Processing %1 [%2]").arg(messageStrings[id]).arg(loginString);
        handleTariffs();
        break;
    default:
        tokenIsGood = validateToken(newPacket->header.accessToken, connection->getSocket().peerAddress());
        break;
    }

    if (!tokenIsGood && !isHandled) {
        qDebug() << QString("Processing Bad Token [%2]").arg(loginString);
        handleBadToken();
    }
    else if (!isHandled) {
        auto userSession = SessionsManager::instance()->getInfo(newPacket->header.accessToken);
        userSession->startSession();

        loginString = userSession->getProfile().login;

        qDebug() << QString("Processing %1 [%2]").arg(messageStrings[id]).arg(loginString);

        switch (id) {
        case MessageID::DeleteFileRequest:
            handleDeletion(userSession, newPacket->data);
            break;
        case MessageID::UploadFileRequest:
            handleUpload(userSession, newPacket->data);
            return;
        case MessageID::FileChunkRequest:
            sendFileChunk();
            return;
        case MessageID::EndOfFile:
            handleEndOfFile(userSession);
            break;
        case MessageID::ProfileRequest:
            handleProfile(userSession, newPacket->data);
            break;
        case MessageID::DownloadFileRequest:
            handleDownload(userSession, newPacket->data);
            return;
        case MessageID::StorageContentRequest:
            handleStorageContent(userSession, newPacket->data);
            break;
        case MessageID::FileChunkResponse:
            receiveFileChunk(newPacket->data);
            return;
        default:
            isHandled = true;
            break;
        }
    }

    if (isHandled) {
        qDebug() << QString("%1 Handled [%2]").arg(messageStrings[id]).arg(loginString);
        emit handled();
    }
}

void ConnectionHandler::onClosed(Connection *connection) {
    Q_UNUSED(connection)

    if (transfer.fileName() != "" && !isHandled && transfer.openMode().testFlag(QIODevice::WriteOnly)) {
        transfer.remove();
    }
}

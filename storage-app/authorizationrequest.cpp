#include "authorizationrequest.h"

#include "protocol.h"
#include "ntconfig.h"
#include "storageinfo.h"
#include "cipher.h"

#include <QCryptographicHash>
#include <QFile>

void ReadCryptoKey(const QString &path);

void AuthorizationRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    QDataStream stream(newPacket->data);
    stream.setVersion(QDataStream::Qt_5_0);

    if (newPacket->header.id == MessageID::Error) {
        auto errorString = new QString;
        stream >> *errorString;
        emit error(errorString);
        return;
    }

    stream >> NtConfig::get()->accessToken;

    StorageInfo::get()->accountName = login;

    try {
        ReadCryptoKey(login + ".crypt");
    }  catch (std::runtime_error &e) {
        emit error(new QString(e.what()));
    }

    emit complete();
}

void ReadCryptoKey(const QString &path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("There's no crypto key!");
    }

    StorageInfo::get()->cryptoKey = file.read(kDefaultKeySize);

    file.close();
}

void AuthorizationRequest::operator()() {
    NetworkPacket packet;
    QDataStream stream(&packet.data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << login << QCryptographicHash::hash(password.toUtf8(),
                                                QCryptographicHash::Sha3_256).toHex();

    auto writer = connection->getWriter();

    writer->write(MessageID::AuthorizationRequest, &packet.data);
}

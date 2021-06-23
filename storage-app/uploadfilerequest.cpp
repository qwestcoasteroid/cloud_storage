#include "uploadfilerequest.h"

#include "protocol.h"
#include "storageinfo.h"
#include "cipher.h"
#include "filereader.h"

#include <QFileInfo>
#include <QApplication>
#include <QDateTime>

void UploadFileRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    if (newPacket->header.id == MessageID::Error) {
        QDataStream stream(newPacket->data);
        stream.setVersion(QDataStream::Qt_5_0);
        auto errorString = new QString;
        stream >> *errorString;
        emit error(errorString);
    }
    if (newPacket->header.id == MessageID::EndOfFile) {
        emit complete();
    }
    if (newPacket->header.id == MessageID::FileChunkRequest) {
        auto writer = connection->getWriter();
        auto chunk = fileReader.read();

        if (chunk.size() != 0) {
            QByteArray data;
            data = cipher.encrypt(chunk);
            sent += chunk.size();
            writer->write(MessageID::FileChunkResponse, &data);
            emit progress(sent * 100 / size);
        }
        else {
            writer->write(MessageID::EndOfFile);
        }
    }
}

void UploadFileRequest::operator()() {
	Cipher cipher(StorageInfo::get()->cryptoKey);
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);
    QFileInfo info(path);

    QByteArray encryptedData;
    QDataStream encryptedStream(&encryptedData, QIODevice::WriteOnly);
    encryptedStream.setVersion(QDataStream::Qt_5_0);

    stream << info.fileName() << info.size() << QDateTime::currentDateTime();

    QByteArray enc = cipher.encrypt(data);

    encryptedStream << enc << info.size();

    auto writer = connection->getWriter();

    writer->write(MessageID::UploadFileRequest, &encryptedData);
}

void UploadFileRequest::stop() {
    connection->disconnect();
}

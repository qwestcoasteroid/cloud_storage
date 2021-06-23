#include "downloadfilerequest.h"

#include "protocol.h"
#include "storageinfo.h"
#include "cipher.h"
#include "filewriter.h"

DownloadFileRequest::DownloadFileRequest(const QString &uuid, const QString &path, qint64 size) :
    uuid(uuid), path(path), fileSize(size), cipher(StorageInfo::get()->cryptoKey),
    fileWriter(path, size) {}

void DownloadFileRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    auto writer = connection->getWriter();

    switch (newPacket->header.id) {
    case MessageID::Error:
        emit error(new QString(QString::fromUtf8(newPacket->data)));
        break;
    case MessageID::FileChunkResponse:
        received += fileWriter.write(cipher.decrypt(newPacket->data));
        writer->write(MessageID::FileChunkRequest);
        emit progress(received * 100 / fileSize);
        break;
    case MessageID::DownloadFileResponse:
        writer->write(MessageID::FileChunkRequest);
        break;
    case MessageID::EndOfFile:
        writer->write(MessageID::EndOfFile);
        emit complete();
        break;
    default:
        break;
    }
}

void DownloadFileRequest::operator()() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << uuid;

    auto writer = connection->getWriter();

    writer->write(MessageID::DownloadFileRequest, &data);
}

void DownloadFileRequest::stop() {
    connection->disconnect();
}

#include "deletefilerequest.h"

void DeleteFileRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    auto *data = new QByteArray;
    QDataStream stream(data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << uuid;

    if (newPacket->header.id == MessageID::Error) {
        emit error(new QString(QString::fromUtf8(newPacket->data)));
        return;
    }

    emit complete(data);
}

void DeleteFileRequest::operator()() {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << uuid;

    auto writer = connection->getWriter();

    writer->write(MessageID::DeleteFileRequest, &data);
}

#include "profilerequest.h"

#include "protocol.h"
#include "ntconfig.h"

void ProfileRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    if (newPacket->header.id == MessageID::Error) {
        emit error(new QString(QString::fromUtf8(newPacket->data)));
        return;
    }

    emit complete(new QByteArray(std::move(newPacket->data)));
}

void ProfileRequest::operator()() {
    auto writer = connection->getWriter();

    writer->write(MessageID::ProfileRequest);
}

QDataStream &operator>>(QDataStream &stream, ProfileResponse &response) {
    stream >> response.name >> response.freeSpace >> response.files;
    return stream;
}

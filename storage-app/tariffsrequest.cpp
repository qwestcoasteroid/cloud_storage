#include "tariffsrequest.h"

void TariffsRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    if (newPacket->header.id == MessageID::Error) {
        emit error(new QString(QString::fromUtf8(newPacket->data)));
        return;
    }

    emit complete(new QByteArray(newPacket->data));
}

void TariffsRequest::operator()() {
    auto writer = connection->getWriter();

    writer->write(MessageID::TariffsRequest);
}

QDataStream &operator>>(QDataStream &stream, Tariff &rhs) {
    stream >> rhs.name >> rhs.space;

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Tariff &rhs) {
    stream << rhs.name << rhs.space;

    return stream;
}

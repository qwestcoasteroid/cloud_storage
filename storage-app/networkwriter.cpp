#include "networkwriter.h"

#include <QDataStream>

#include "ntconfig.h"

NetworkWriter::NetworkWriter(QTcpSocket *socket, QObject *parent) :
    QObject(parent), socket(socket) {}

void NetworkWriter::write(MessageID id, QByteArray *bytes) {
    NetworkPacket packet;
    packet.header.id = id;
    packet.header.size = bytes ? bytes->size() : 0;
    packet.header.accessToken = NtConfig::get()->accessToken;

    if (bytes) {
        packet.data = *bytes;
    }

    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << packet;
}

#include "networkreader.h"

#include <QDataStream>

#include <stdexcept>

#include "ntconfig.h"

NetworkReader::NetworkReader(QTcpSocket *socket, QObject *parent) :
    QObject(parent), socket(socket) {

    QAbstractSocket::connect(socket, &QAbstractSocket::readyRead, this, &NetworkReader::onReadyRead);
}

void NetworkReader::onReadyRead() {
    QDataStream stream(socket);
    stream.setVersion(QDataStream::Qt_5_0);
    stream.startTransaction();

    auto result = std::make_shared<NetworkPacket>();

    stream >> *result;

    if (!stream.commitTransaction()) {
        return;
    }

    emit packetReady(result);
}

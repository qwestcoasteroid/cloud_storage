#ifndef NETWORKREADER_H
#define NETWORKREADER_H

#include <QtNetwork/QTcpSocket>

#include "protocol.h"

class NetworkReader : public QObject {
    Q_OBJECT

public:
    explicit NetworkReader(QTcpSocket *socket, QObject *parent = 0);
    ~NetworkReader() = default;

public slots:
    void onReadyRead();

signals:
    void packetReady(std::shared_ptr<NetworkPacket> packet);

private:
    QTcpSocket *socket;
};

#endif // NETWORKREADER_H

#ifndef NETWORKWRITER_H
#define NETWORKWRITER_H

#include <QtNetwork/QTcpSocket>

#include "protocol.h"

class NetworkWriter : public QObject {
    Q_OBJECT

public:
    explicit NetworkWriter(QTcpSocket *socket, QObject *parent = 0);
    ~NetworkWriter() = default;

    void write(MessageID id, QByteArray *bytes = nullptr);

private slots:
    void onBytesWritten(quint64 bytes);

private:
    QTcpSocket *socket;
    quint64 lastPacketSize{};
    bool canWrite{ true };
};

#endif // NETWORKWRITER_H

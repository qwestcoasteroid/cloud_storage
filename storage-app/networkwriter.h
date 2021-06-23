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

private:
    QTcpSocket *socket;
};

#endif // NETWORKWRITER_H

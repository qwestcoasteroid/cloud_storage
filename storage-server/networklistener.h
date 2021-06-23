#ifndef NETWORKLISTENER_H
#define NETWORKLISTENER_H

#include <QtNetwork/QTcpServer>

class NetworkListener : public QTcpServer {
    Q_OBJECT

public:
    NetworkListener(const QHostAddress &address, quint16 port, QObject *parent = 0);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
};

#endif // NETWORKLISTENER_H

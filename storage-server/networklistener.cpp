#include "networklistener.h"
#include "connectionhandler.h"

#include <QThread>
#include <QDebug>

NetworkListener::NetworkListener(const QHostAddress &address, quint16 port, QObject *parent) :
    QTcpServer(parent) {

    if (listen(address, port)) {
        qDebug() << "Server started at port: " << serverPort();
    }
    else {
         qDebug() << "Server not started!";
    }
}

void NetworkListener::incomingConnection(qintptr socketDescriptor) {
    auto handler = new ConnectionHandler(socketDescriptor);

    connect(handler, &ConnectionHandler::handled, handler, &ConnectionHandler::deleteLater);
}

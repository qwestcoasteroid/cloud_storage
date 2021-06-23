#include "connection.h"

#include "ntconfig.h"

#include <stdexcept>
#include <QHostAddress>

Connection::Connection(QObject *parent) : QObject(parent),
    socket(new QTcpSocket), reader(new NetworkReader(socket)),
    writer(new NetworkWriter(socket)) {

    const auto &config = NtConfig::get();

    socket->connectToHost(config->serverAddress, config->serverPort.toUShort());

    if (!socket->waitForConnected(10000)) {
        qDebug() << socket->errorString();
        throw std::runtime_error("Unable to connect to the server!");
    }
}

Connection::Connection(quintptr socketDescriptor, QObject *parent) :
    QObject(parent), socket(new QTcpSocket), reader(new NetworkReader(socket)),
    writer(new NetworkWriter(socket)) {

    socket->setSocketDescriptor(socketDescriptor);

    qDebug() << QString("Client connected [%1:%2]").arg(socket->peerAddress().toString()).arg(socket->peerPort());

    connect(socket, &QTcpSocket::disconnected, this, &Connection::forwardDisconnect);
}

Connection::~Connection() {
    if (socket->isOpen()) {
        socket->close();
    }
}

void Connection::disconnect() {
    socket->disconnectFromHost();
}

NetworkReader *Connection::getReader() {
    return reader;
}

NetworkWriter *Connection::getWriter() {
    return writer;
}

void Connection::forwardDisconnect() {
    emit connectionClosed(this);
}

bool Connection::operator==(const Connection &rhs) {
    return socket->socketDescriptor() == rhs.socket->socketDescriptor();
}

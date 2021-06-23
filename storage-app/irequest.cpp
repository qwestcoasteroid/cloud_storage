#include "irequest.h"

#include <QThread>

void IRequest::execute() {
    connection = new Connection(this);
    connect(connection->getReader(), &NetworkReader::packetReady, this, &IRequest::onPacketReady);
    connect(connection, &Connection::connectionClosed, connection, &Connection::deleteLater);
    this->operator()();
}

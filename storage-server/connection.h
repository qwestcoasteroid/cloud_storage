#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork/QTcpSocket>

#include <memory>

#include "networkreader.h"
#include "networkwriter.h"

class Connection : public QObject {
    Q_OBJECT

public:
    Connection(QObject *parent = 0);
    Connection(quintptr socketDescriptor, QObject *parent = 0);
    ~Connection();

    NetworkReader *getReader();
    NetworkWriter *getWriter();

    const QTcpSocket &getSocket() const;

    void disconnect();

    bool operator==(const Connection &rhs);

public slots:
    void forwardDisconnect();

signals:
    void connectionClosed(Connection *connection);

private:
    QTcpSocket *socket;
    NetworkReader *reader;
    NetworkWriter *writer;
};

#endif // CONNECTION_H

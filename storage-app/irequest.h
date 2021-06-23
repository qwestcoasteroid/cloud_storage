#ifndef IREQUEST_H
#define IREQUEST_H

#include <QObject>
#include <memory>

#include "connection.h"

class IRequest : public QObject {
    Q_OBJECT

public:
    void execute();

public slots:
    virtual void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) = 0;

signals:
    void complete(QByteArray *data = nullptr);
    void error(QString *error = nullptr);

protected:
    virtual void operator()() = 0;

    Connection *connection;
};

#endif // IREQUEST_H

#ifndef TARIFFSREQUEST_H
#define TARIFFSREQUEST_H

#include "irequest.h"

struct Tariff {
    QString name;
    qint64 space;
};

QDataStream &operator>>(QDataStream &stream, Tariff &rhs);
QDataStream &operator<<(QDataStream &stream, const Tariff &rhs);

class TariffsRequest : public IRequest {
    Q_OBJECT

public:
    TariffsRequest() = default;

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;

protected:
    void operator()() override;
};

#endif // TARIFFSREQUEST_H

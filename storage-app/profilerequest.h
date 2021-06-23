#ifndef PROFILEREQUEST_H
#define PROFILEREQUEST_H

#include "irequest.h"

#include <QDataStream>

struct ProfileResponse {
    QString name;
    qint64 freeSpace;
    quint32 files;
};

QDataStream &operator>>(QDataStream &stream, ProfileResponse &response);

class ProfileRequest : public IRequest {
public:
    ProfileRequest() = default;

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;

protected:
    void operator()() override;
};

#endif // PROFILEREQUEST_H

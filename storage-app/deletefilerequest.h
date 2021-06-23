#ifndef DELETEFILEREQUEST_H
#define DELETEFILEREQUEST_H

#include "irequest.h"

class DeleteFileRequest : public IRequest {
public:
    explicit DeleteFileRequest(const QString &uuid) :
        uuid(uuid) {}

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;

protected:
    void operator()() override;

private:
    QString uuid;
};

#endif // DELETEFILEREQUEST_H

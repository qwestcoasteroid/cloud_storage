#ifndef AUTHORIZATIONREQUEST_H
#define AUTHORIZATIONREQUEST_H

#include "irequest.h"

class AuthorizationRequest : public IRequest {
public:
    AuthorizationRequest(const QString &login, const QString password) :
        login(login), password(password) {}

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;

protected:
    void operator()() override;

private:
    QString login;
    QString password;
};

#endif // AUTHORIZATIONREQUEST_H

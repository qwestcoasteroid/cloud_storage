#ifndef REGISTRATIONREQUEST_H
#define REGISTRATIONREQUEST_H

#include "irequest.h"

class RegistrationRequest : public IRequest {
public:
    RegistrationRequest(const QString &login, const QString password, const QString tariff) :
        login(login), password(password), tariff(tariff) {}

    void operator()() override;

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;

private:
    QString login;
    QString password;
    QString tariff;
};

#endif // REGISTRATIONREQUEST_H

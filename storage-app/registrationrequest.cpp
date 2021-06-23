#include "registrationrequest.h"

#include "protocol.h"
#include "ntconfig.h"
#include "cipher.h"

#include <QCryptographicHash>
#include <QFile>

void RegistrationRequest::onPacketReady(std::shared_ptr<NetworkPacket> newPacket) {
    if (newPacket->header.id == MessageID::Error) {
        QDataStream stream(&newPacket->data, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);
        auto errorString = new QString;
        stream >> *errorString;
        QString asd = errorString->toUtf8();
        emit error(errorString);
        return;
    }

    auto cryptoKey = Cipher::generateKey();

    QFile file(login + ".crypt");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(cryptoKey.data(), cryptoKey.size());
    file.close();

    emit complete();
}

void RegistrationRequest::operator()() {
    NetworkPacket packet;
    QDataStream stream(&packet.data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << login << QCryptographicHash::hash(password.toUtf8(),
                                                QCryptographicHash::Sha3_256).toHex()
           << tariff;

    auto writer = connection->getWriter();

    writer->write(MessageID::RegistrationRequest, &packet.data);
}

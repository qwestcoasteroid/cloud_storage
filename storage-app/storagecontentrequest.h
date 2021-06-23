#ifndef STORAGECONTENTREQUEST_H
#define STORAGECONTENTREQUEST_H

#include "irequest.h"

#include <QDateTime>
#include <QDataStream>

struct StorageFileStructure {
    QString name;
    qint64 size;
    QDateTime date;
    QString uuid;
};

bool operator==(const StorageFileStructure &lhs, const StorageFileStructure &rhs);

QDataStream &operator>>(QDataStream &stream, StorageFileStructure &file);
QDataStream &operator<<(QDataStream &stream, StorageFileStructure &file);

class StorageContentRequest : public IRequest {
public:
    StorageContentRequest() = default;

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket) override;

protected:
    void operator()() override;
};

#endif // STORAGECONTENTREQUEST_H

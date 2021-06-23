#ifndef STORAGEINFO_H
#define STORAGEINFO_H

#include <memory>

#include <QByteArray>
#include <QString>

#include "authorizationrequest.h"

class StorageInfo {
public:
    static std::shared_ptr<StorageInfo> get();

    QByteArray cryptoKey;
    QString accountName;

private:
    StorageInfo() = default;
};

#endif // STORAGEINFO_H

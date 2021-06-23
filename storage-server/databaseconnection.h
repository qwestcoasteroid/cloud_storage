#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QtSql>
#include <memory>
#include <optional>

#include "sessionsmanager.h"

struct UserInfo {
    QString login;
    quint64 freeSpace;
    quint32 files;
};

struct Tariff {
    QString name;
    qint64 space;
};

class DataBaseConnection {
public:
    static std::unique_ptr<DataBaseConnection> &instance();

    bool userExists(const QString &user);
    std::shared_ptr<UserInfo> getUserInfo(const QString &user);
    bool addUser(const QString &user, const QString &password, quint32 tariff);
    bool validateUser(const QString &user, const QString &password);
    bool fileExists(quint32 userId, const QString &uuid);
    std::shared_ptr<ProfileInfo> getProfileInfo(const QString &user);
    bool deleteFile(const QString &uuid);
    bool addFile(quint32 id, const QString &uuid);
    bool updateStatistics(quint32 id, qint64 size);
    QList<QString> getFiles(quint32 id);
    QList<Tariff> getTariffs();
    bool setTariff(const QString &user, quint32 tariffId);
    std::optional<quint32> getTariffId(const QString &tariff);

    QString getError();

private:
    DataBaseConnection();

    QSqlDatabase db;
};

#endif // DATABASECONNECTION_H

#include "databaseconnection.h"

#include <QFile>
#include <QDebug>

std::shared_ptr<ProfileInfo> DataBaseConnection::getProfileInfo(const QString &user) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE login=:login");
    query.bindValue(":login", user);
    query.exec();

    std::shared_ptr<ProfileInfo> result;

    if (query.next()) {
        result = std::make_shared<ProfileInfo>();
        result->id = query.value(0).toUInt();
        result->login = query.value(1).toString();
        result->hash = query.value(2).toString();
        result->tariff = query.value(3).toUInt();
    }

    return result;
}

QList<Tariff> DataBaseConnection::getTariffs() {
    QSqlQuery query;
    query.prepare("SELECT name, space FROM tariffs");
    query.exec();

    QList<Tariff> result;

    while (query.next()) {
        result.append({ query.value(0).toString(), query.value(1).toLongLong() });
    }

    return result;
}

bool DataBaseConnection::setTariff(const QString &user, quint32 tariffId) {
    QSqlQuery query;
    query.prepare("UPDATE users SET tariff=:tariff WHERE login=:user");
    query.bindValue(":tariff", tariffId);
    query.bindValue(":user", user);

    return query.exec();
}

bool DataBaseConnection::validateUser(const QString &user, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT EXISTS (SELECT id FROM users WHERE login=:login "
        "AND password_hash=:password)");
    query.bindValue(":login", user);
    query.bindValue(":password", password);
    query.exec();
    query.first();

    return query.value(0).toUInt() == 1;
}

bool DataBaseConnection::userExists(const QString &user) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE login=:login");
    query.bindValue(":login", user);
    query.exec();

    return query.next();
}

std::shared_ptr<UserInfo> DataBaseConnection::getUserInfo(const QString &user) {
    QSqlQuery query;
    query.prepare("SELECT users.login, statistics.used_space, "
        "statistics.files, tariffs.space FROM users JOIN statistics ON users.id=statistics.id "
        " JOIN tariffs ON users.tariff=tariffs.id WHERE users.login=:login");
    query.bindValue(":login", user);
    query.exec();

    std::shared_ptr<UserInfo> result;

    if (query.next()) {
        result = std::make_shared<UserInfo>();
        result->login = query.value(0).toString();
        result->freeSpace = query.value(3).toLongLong() - query.value(1).toLongLong();
        result->files = query.value(2).toUInt();
    }

    return result;
}

bool DataBaseConnection::deleteFile(const QString &uuid) {
    QSqlQuery query;
    query.prepare("DELETE FROM files WHERE name=:uuid");
    query.bindValue(":uuid", uuid);

    return query.exec();
}

bool DataBaseConnection::addFile(quint32 id, const QString &uuid) {
    QSqlQuery query;
    query.prepare("INSERT INTO files (id, name) VALUES (:id, :uuid)");
    query.bindValue(":id", id);
    query.bindValue(":uuid", uuid);

    return query.exec();
}

bool DataBaseConnection::updateStatistics(quint32 id, qint64 size) {
    QSqlQuery query;
    query.prepare("UPDATE statistics SET used_space=used_space+:size WHERE id=:id");
    query.bindValue(":size", size);
    query.bindValue(":id", id);

    return query.exec();
}

std::optional<quint32> DataBaseConnection::getTariffId(const QString &tariff) {
    QSqlQuery query;
    query.prepare("SELECT id FROM tariffs WHERE name=:name");
    query.bindValue(":name", tariff);
    query.exec();

    std::optional<quint32> result;

    if (query.next()) {
        result = query.value(0).toUInt();
    }

    return result;
}

bool DataBaseConnection::addUser(const QString &user, const QString &password, quint32 tariff) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (login, password_hash, tariff) VALUES (:login, :pass, :tariff)");
    query.bindValue(":login", user);
    query.bindValue(":pass", password);
    query.bindValue(":tariff", tariff);

    bool result = query.exec();

    return result;
}

bool DataBaseConnection::fileExists(quint32 userId, const QString &uuid) {
    QSqlQuery query;
    query.prepare("SELECT EXISTS (SELECT id FROM files WHERE id=:id AND name=:uuid)");
    query.bindValue(":id", userId);
    query.bindValue(":uuid", uuid);
    query.exec();

    query.next();

    return query.value(0) == 1;
}

QList<QString> DataBaseConnection::getFiles(quint32 id) {
    QList<QString> result;
    QSqlQuery query;
    query.prepare("SELECT name FROM files WHERE id=:id");
    query.bindValue(":id", id);
    query.exec();

    while (query.next()) {
        result.append(query.value(0).toString());
    }

    return result;
}

std::unique_ptr<DataBaseConnection> &DataBaseConnection::instance() {
    static std::unique_ptr<DataBaseConnection> instance(new DataBaseConnection);

    return instance;
}

DataBaseConnection::DataBaseConnection() :
    db(QSqlDatabase::addDatabase("QSQLITE")) {

    db.setDatabaseName("data.db");

    if (!db.open()) {
        throw std::runtime_error("Database not found!");
    }

    QFile migrationFile("migration.sql");

    if (!migrationFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Migration not found!");
    }

    QString migrationString = QString::fromUtf8(migrationFile.readAll());

    auto queryStrings = migrationString.split("#", Qt::SkipEmptyParts);

    for (const auto &item : queryStrings) {
        QString trimmedString = item.trimmed();
        QSqlQuery query(trimmedString);
        if (trimmedString.size() == 0 ? false : !query.exec()) {
            throw std::runtime_error("Bad migration!");
        }
    }

    migrationFile.close();
}

QString DataBaseConnection::getError() {
    return db.lastError().text();
}

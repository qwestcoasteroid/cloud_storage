#include "sessionsmanager.h"
#include "protocol.h"

#include <random>
#include <QtDebug>

bool operator==(const ProfileInfo &lhs, const ProfileInfo &rhs) {
    return lhs.id == rhs.id;
}

bool operator==(const SessionInfo &lhs, const SessionInfo &rhs) {
    return lhs.getProfile() == rhs.getProfile();
}

SessionInfo::SessionInfo(const ProfileInfo &profile, const QHostAddress &address, QObject *parent) :
    QObject(parent), profile(profile), address(address) {

    timer = std::make_shared<QTimer>();
    timer->setInterval(sessionDuration);
    timer->callOnTimeout(this, &SessionInfo::forwardTimeout);
    timer->start();
}

const ProfileInfo &SessionInfo::getProfile() const {
    return profile;
}

void SessionInfo::startSession() {
    timer->start();
}

void SessionInfo::forwardTimeout() {
    emit sessionEnded(this);
}

QString generateAccessToken() {
    QString result;
    result.reserve(kAccessTokenSize);

    static const QString str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<int> dist(0, str.size() - 1);

    for (int i = 0; i < kAccessTokenSize; ++i) {
        result += str[dist(generator)];
    }

    return result;
}

SessionsManager::SessionsManager(QObject *parent) : QObject(parent) {}

std::shared_ptr<SessionsManager> SessionsManager::instance() {
    static std::shared_ptr<SessionsManager> manager(new SessionsManager);

    return manager;
}

QString SessionsManager::addSession(const ProfileInfo &profile, const QHostAddress &address) {
    QString newToken;
    bool stop{ false };

    qDebug() << QString("Generating access token [%1]").arg(profile.login);

    while (!stop) {
        newToken = generateAccessToken();

        if ((stop = !sessions.contains(newToken))) {
            qDebug() << QString("Access token generated [%1]").arg(profile.login);
            sessions.insert(newToken, { profile, address });
            connect(&sessions[newToken], &SessionInfo::sessionEnded, this, &SessionsManager::remove);
        }
    }

    qDebug() << QString("Session started [%1]").arg(profile.login);

    return newToken;
}

void SessionsManager::removeSession(const QString &token) {
    sessions.remove(token);
}

void SessionsManager::remove(SessionInfo *session) {
    QString key = sessions.key(*session);

    if (key.size() != 0) {
        qDebug() << QString("Session expired [%1]").arg(session->getProfile().login);
        sessions.remove(key);
        delete session;
    }
}

SessionInfo *SessionsManager::getInfo(const QString &token) {
    if (sessions.contains(token)) {
        return &sessions[token];
    }

    return nullptr;
}

QString SessionsManager::getToken(const ProfileInfo &profile, const QHostAddress &address) {
    return sessions.key({ profile, address });
}

const QHostAddress &SessionInfo::getAddress() const {
    return address;
}

void SessionInfo::setAddress(const QHostAddress &address) {
    this->address = address;
}

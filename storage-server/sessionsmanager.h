#ifndef SESSIONSMANAGER_H
#define SESSIONSMANAGER_H

#include <memory>
#include <QMap>
#include <QTimer>
#include <QHostAddress>

struct ProfileInfo {
    quint32 id{};
    QString login{};
    QString hash{};
    quint8 tariff{};
};

bool operator==(const ProfileInfo &lhs, const ProfileInfo &rhs);

class SessionInfo : public QObject {
    Q_OBJECT

public:
    static constexpr std::chrono::milliseconds sessionDuration = std::chrono::hours(1);

    SessionInfo(QObject *parent = 0) : QObject(parent) {}
    SessionInfo(const ProfileInfo &profile, const QHostAddress &address, QObject *parent = 0);
    SessionInfo(const SessionInfo &info) : profile(info.profile),
        address(info.address), timer(info.timer) {}

    void startSession();

    const ProfileInfo &getProfile() const;
    void setProfileInfo(const ProfileInfo &profile);

    const QHostAddress &getAddress() const;
    void setAddress(const QHostAddress &address);

    SessionInfo &operator=(const SessionInfo &rhs) {
        if (this == &rhs) {
            return *this;
        }

        profile = rhs.profile;
        address = rhs.address;
        timer = rhs.timer;

        return *this;
    }

signals:
    void sessionEnded(SessionInfo *session);

private slots:
    void forwardTimeout();

private:
    ProfileInfo profile;
    QHostAddress address;
    std::shared_ptr<QTimer> timer;
};

bool operator==(const SessionInfo &lhs, const SessionInfo &rhs);

class SessionsManager : public QObject {
    Q_OBJECT

public:
    static std::shared_ptr<SessionsManager> instance();
    QString addSession(const ProfileInfo &profile, const QHostAddress &address);
    void removeSession(const QString &token);
    SessionInfo *getInfo(const QString &token);
    QString getToken(const ProfileInfo &profile, const QHostAddress &address);

private slots:
    void remove(SessionInfo *session);

private:
    SessionsManager(QObject *parent = 0);

    QMap<QString, SessionInfo> sessions;
};

#endif // SESSIONSMANAGER_H

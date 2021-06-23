#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <QFile>

#include "connection.h"

#include "sessionsmanager.h"

class ConnectionHandler : public QObject {
    Q_OBJECT

public:
    ConnectionHandler(quintptr socketDescriptor, QObject *parent = 0);

public slots:
    void onPacketReady(std::shared_ptr<NetworkPacket> newPacket);
    void onClosed(Connection *connection);

signals:
    void handled();

private:
    void startHandling();

    void handleRegistration(const QByteArray &data);
    void handleAuthorization(const QByteArray &data);
    void handleDeletion(SessionInfo *session, const QByteArray &data);
    void handleDownload(SessionInfo *session, const QByteArray &data);
    void handleProfile(SessionInfo *session, const QByteArray &data);
    void handleStorageContent(SessionInfo *session, const QByteArray &data);
    void handleUpload(SessionInfo *session, const QByteArray &data);
    void sendFileChunk();
    void receiveFileChunk(const QByteArray &data);
    void handleEndOfFile(SessionInfo *session);
    void handleTariffs();
    void handleBadToken();

    quintptr socketDescriptor;
    Connection *connection;
    QFile transfer;
    QDataStream fileStream;
    bool isHandled { false };
};

#endif // CONNECTIONHANDLER_H

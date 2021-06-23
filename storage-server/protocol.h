#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>
#include <QString>
#include <QDataStream>

#include <array>
#include <QMap>

extern const quint16 kAccessTokenSize;

enum class MessageID : quint16 {
    AuthorizationRequest, RegistrationRequest, ProfileRequest,
    StorageContentRequest, DownloadFileRequest, UploadFileRequest,
    EndOfFile, DeleteFileRequest, FileChunkRequest, Error,
    FileChunkResponse, DownloadFileResponse, TariffsRequest
};

extern const QMap<MessageID, QString> messageStrings;

struct NetworkHeader {
    MessageID id;
    quint32 size;
    QString accessToken;
};

struct NetworkPacket {
    NetworkHeader header;
    QByteArray data;
};

QDataStream &operator<<(QDataStream &stream, const NetworkHeader &header);
QDataStream &operator>>(QDataStream &stream, NetworkHeader &header);
QDataStream &operator<<(QDataStream &stream, const NetworkPacket &packet);
QDataStream &operator>>(QDataStream &stream, NetworkPacket &packet);

#endif // PROTOCOL_H

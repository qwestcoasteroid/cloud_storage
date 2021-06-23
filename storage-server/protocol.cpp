#include "protocol.h"

#include <type_traits>

const quint16 kAccessTokenSize = 32;

const QMap<MessageID, QString> messageStrings {
    { MessageID::AuthorizationRequest, "Authorization Request" },
    { MessageID::DeleteFileRequest, "File Deletion Request" },
    { MessageID::DownloadFileRequest, "File Downloading Request" },
    { MessageID::EndOfFile, "End of Transfer" },
    { MessageID::FileChunkRequest, "File Part To Client" },
    { MessageID::ProfileRequest, "Profile Info Request" },
    { MessageID::RegistrationRequest, "Registration Request" },
    { MessageID::StorageContentRequest, "Storage Content Request" },
    { MessageID::UploadFileRequest, "File Uploading Request" },
    { MessageID::TariffsRequest, "Tariffs Request" },
    { MessageID::FileChunkResponse, "File Part From Client" }
};

QDataStream &operator<<(QDataStream &stream, const NetworkHeader &header) {
    stream << static_cast<std::underlying_type<MessageID>::type>(header.id)
           << header.size << header.accessToken;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, NetworkHeader &header) {
    std::underlying_type<MessageID>::type messageID{};

    stream >> messageID >> header.size >> header.accessToken;

    header.id = static_cast<MessageID>(messageID);

    return stream;
}

QDataStream &operator<<(QDataStream &stream, const NetworkPacket &packet) {
    stream << packet.header << packet.data;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, NetworkPacket &packet) {
    stream >> packet.header >> packet.data;

    return stream;
}

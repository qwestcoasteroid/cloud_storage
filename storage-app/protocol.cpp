#include "protocol.h"

#include <type_traits>

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

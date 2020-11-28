#ifndef CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
#define CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_

#include <memory>
#include <string_view>
#include <cstring>

// TODO: MakeError - send error code instead of message

namespace cloud_storage::network {
    enum class DataType : uint8_t {
        kFileInfo       = 0xFF, kQuery          = 0xFE,
        kAuthorization  = 0xFD, kRegistration   = 0xFC,
        kProfileInfo    = 0xFB, kMessage        = 0xFA
    };

    enum class ErrorCode : uint8_t {
        kSuccess = 0x00, kError = 0x01
    };

    struct NetworkHeader {
        uint32_t access_token{};
        uint32_t data_length{};
        uint32_t user_id{};
        DataType data_type;
        ErrorCode error_code{ ErrorCode::kSuccess };
    };

    struct NetworkBuffer {
        uint32_t length{};
        std::shared_ptr<char[]> buffer;
        DataType data_type;
    };

    struct SessionInfo {
        uint32_t access_token{};
        uint32_t user_id{};
    };

    struct Packet {
        NetworkHeader header;
        std::shared_ptr<char[]> data;
    };

    void PackNetworkHeader(NetworkHeader &_header, bool _to_network) noexcept;

    Packet MakeRequest(DataType _type, std::string_view _resource);
    Packet MakeRespond(DataType _type,
        const std::shared_ptr<char[]> &_buffer, size_t _size);
    Packet MakeError(DataType _type, std::string_view _message);
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
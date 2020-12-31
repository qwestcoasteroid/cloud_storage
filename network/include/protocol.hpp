#ifndef CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
#define CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_

#include <memory>
#include <cstdint>

namespace cloud_storage::network {
    enum class MessageID : uint8_t {
        kFileInfoRequest       = 0xFF, kFileInfoResponse        = 0xFE,
        kAuthorizationRequest  = 0xFD, kAuthorizationResponse   = 0xFC,
        kRegistrationRequest   = 0xFB, kRegistrationResponse    = 0xFA,
        kProfileInfoRequest    = 0xF9, kProfileInfoResponse     = 0xF8,
        kFileUploadRequest     = 0xF7, kFileUploadResponse      = 0xF6,
        kFileDownloadRequest   = 0xF5, kFileDownloadResponse    = 0xF4,
        kFilePart              = 0xF3
    };

    enum class ErrorCode : uint8_t {
        kSuccess = 0x00, kError = 0x01, kNotEnoughSpace = 0x02
    };

    struct NetworkHeader {
        uint32_t access_token{};
        uint32_t data_length{};
        uint32_t user_id{};
        MessageID message_id;
        ErrorCode error_code{ ErrorCode::kSuccess };
    };

    struct NetworkBuffer {
        uint32_t length{};
        std::shared_ptr<char[]> buffer;
        MessageID message_id;
    };

    struct SessionInfo {
        uint32_t access_token{};
        uint32_t user_id{};
    };

    struct Packet {
        NetworkHeader header;
        std::shared_ptr<char[]> data;
    };

    void SwitchByteOrder(NetworkHeader &_header) noexcept;
    void AppendPacket(Packet &_packet, const NetworkBuffer &_buffer) noexcept;
    NetworkBuffer CreateNetworkBuffer(const Packet &_packet) noexcept;
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
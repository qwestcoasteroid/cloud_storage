#ifndef CLOUD_STORAGE_MESSAGES_FILE_INFO_HPP_
#define CLOUD_STORAGE_MESSAGES_FILE_INFO_HPP_

#include <winbase.h>

#include <cstdint>
#include <string>

#include "serializable.hpp"
#include "protocol.hpp"

namespace cloud_storage::messages {
    class FileInfoResponse : public service::Serializable {
    public:
        inline FileInfoResponse() noexcept {}
        FileInfoResponse(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FileInfoResponse &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string name;
        uint64_t size{};
        SYSTEMTIME creation_time{};
        SYSTEMTIME editing_time{};
    };

    class FileInfoRequest : public service::Serializable {
    public:
        inline FileInfoRequest() noexcept {}
        FileInfoRequest(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FileInfoRequest &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string name;
    };
} // namespace cloud_storage::messages

#endif // CLOUD_STORAGE_MESSAGES_FILE_INFO_HPP_
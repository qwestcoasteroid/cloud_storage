#ifndef CLOUD_STORAGE_MESSAGES_FILE_UPLOAD_HPP_
#define CLOUD_STORAGE_MESSAGES_FILE_UPLOAD_HPP_

#include <cstdint>
#include <string>

#include "serializable.hpp"
#include "protocol.hpp"

namespace cloud_storage::messages {
    class FileUploadResponse : public service::Serializable {
    public:
        inline FileUploadResponse() noexcept {}
        FileUploadResponse(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FileUploadResponse &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string name;
        uint64_t difference{};
        uint32_t operation_id{};
        uint32_t chunk_size{};
    };

    class FileUploadRequest : public service::Serializable {
    public:
        inline FileUploadRequest() noexcept {}
        FileUploadRequest(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FileUploadRequest &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string name;
        uint64_t size{};
    };
} // namespace cloud_storage::messages

#endif // CLOUD_STORAGE_MESSAGES_FILE_UPLOAD_HPP_
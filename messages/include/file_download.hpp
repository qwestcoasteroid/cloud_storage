#ifndef CLOUD_STORAGE_MESSAGES_FILE_DOWNLOAD_H_
#define CLOUD_STORAGE_MESSAGES_FILE_DOWNLOAD_H_

#include <cstdint>
#include <memory>

#include "serializable.hpp"
#include "protocol.hpp"

namespace cloud_storage::messages {
    class FileDownloadResponse : public service::Serializable {
    public:
        inline FileDownloadResponse() noexcept {}
        FileDownloadResponse(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FileDownloadResponse &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string name;
        uint64_t size;
        uint32_t operation_id{};
        uint32_t chunk_size{};
    };

    class FileDownloadRequest : public service::Serializable {
    public:
        inline FileDownloadRequest() noexcept {}
        FileDownloadRequest(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FileDownloadRequest &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string name;
    };
} // namespace cloud_storage::messages

#endif // CLOUD_STORAGE_MESSAGES_FILE_DOWNLOAD_H_
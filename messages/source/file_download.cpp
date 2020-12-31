#include "file_download.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::messages {
    FileDownloadResponse::FileDownloadResponse(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer FileDownloadResponse::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kFileDownloadResponse;

        result.length += sizeof(operation_id) + sizeof(size) +
            sizeof(chunk_size) + name.size() + 1;

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, name.c_str());
        buffer_ptr += name.size() + 1;

        *reinterpret_cast<decltype(size) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(size);
        buffer_ptr += sizeof(size);

        *reinterpret_cast<decltype(operation_id) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(operation_id);
        buffer_ptr += sizeof(operation_id);

        *reinterpret_cast<decltype(chunk_size) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(chunk_size);

        return result;
    }

    FileDownloadResponse &FileDownloadResponse::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kFileDownloadResponse) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        name = std::string(buffer_ptr);
        buffer_ptr += name.size() + 1;

        size = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(size) *>(buffer_ptr)
        );
        buffer_ptr += sizeof(size);

        operation_id = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(operation_id) *>(buffer_ptr)
        );
        buffer_ptr += sizeof(operation_id);

        chunk_size = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(chunk_size) *>(buffer_ptr)
        );

        return *this;
    }

    FileDownloadRequest::FileDownloadRequest(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer FileDownloadRequest::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kFileDownloadRequest;

        result.length = name.size() + 1;

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, name.c_str());

        return result;
    }

    FileDownloadRequest &FileDownloadRequest::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kFileDownloadRequest) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        name = std::string(buffer_ptr);

        return *this;
    }
} // namespace cloud_storage::messages
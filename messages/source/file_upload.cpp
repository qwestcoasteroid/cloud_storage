#include "file_upload.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::messages {
    FileUploadResponse::FileUploadResponse(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer FileUploadResponse::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kFileUploadResponse;

        result.length += sizeof(operation_id) + sizeof(difference) +
            sizeof(chunk_size) + name.size() + 1;

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, name.c_str());
        buffer_ptr += name.size() + 1;

        *reinterpret_cast<decltype(operation_id) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(operation_id);
        buffer_ptr += sizeof(operation_id);

        *reinterpret_cast<decltype(difference) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(difference);
        buffer_ptr += sizeof(difference);

        *reinterpret_cast<decltype(chunk_size) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(chunk_size);

        return result;
    }

    FileUploadResponse &FileUploadResponse::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kFileUploadResponse) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        name = std::string(buffer_ptr);
        buffer_ptr += name.size() + 1;

        operation_id = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(operation_id) *>(buffer_ptr)
        );
        buffer_ptr += sizeof(operation_id);

        difference = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(difference) *>(buffer_ptr)
        );
        buffer_ptr += sizeof(difference);

        chunk_size = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(chunk_size) *>(buffer_ptr)
        );

        return *this;
    }

    FileUploadRequest::FileUploadRequest(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer FileUploadRequest::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kFileUploadRequest;

        result.length = name.size() + 1 + sizeof(size);

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, name.c_str());
        buffer_ptr += name.size() + 1;

        *reinterpret_cast<decltype(size) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(size);

        return result;
    }

    FileUploadRequest &FileUploadRequest::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kFileUploadRequest) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        name = std::string(buffer_ptr);
        buffer_ptr += name.size() + 1;

        size = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(size) *>(buffer_ptr)
        );

        return *this;
    }
} // namespace cloud_storage::messages
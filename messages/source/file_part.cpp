#include "file_part.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::messages {
    FilePart::FilePart(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer FilePart::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kFilePart;

        result.length = sizeof(size) + size + sizeof(operation_id);
    
        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        *reinterpret_cast<decltype(operation_id) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(operation_id);
        buffer_ptr += sizeof(operation_id);

        *reinterpret_cast<decltype(size) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(size);
        buffer_ptr += sizeof(size);

        std::memcpy(buffer_ptr, buffer.get(), size);

        return result;
    }

    FilePart &FilePart::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kFilePart) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        operation_id = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(operation_id) *>(buffer_ptr)
        );
        buffer_ptr += sizeof(operation_id);

        size = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(size) *>(buffer_ptr)
        );
        buffer_ptr += sizeof(size);

        buffer.reset(new char[size]);

        std::memcpy(buffer.get(), buffer_ptr, size);

        return *this;
    }
} // namespace cloud_storage::messages
#include "message.hpp"

#include <cstring>

namespace cloud_storage::stored_data {
    Message::Message(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer Message::Serialize() const {
        network::NetworkBuffer result;

        result.data_type = network::DataType::kMessage;

        result.length = 0;

        result.length += msg.size() + 1;
        result.length += sender.size() + 1;

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, msg.c_str());
        buffer_ptr += msg.size() + 1;
        std::strcpy(buffer_ptr, sender.c_str());

        return result;
    }

    Message &Message::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.data_type != network::DataType::kMessage) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        msg = std::string(buffer_ptr);
        buffer_ptr += msg.size() + 1;
        sender = std::string(buffer_ptr);

        return *this;
    }
} // namespace cloud_storage::stored_data
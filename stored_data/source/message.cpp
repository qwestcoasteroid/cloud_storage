#include "message.hpp"

#include <cstring>

namespace cloud_storage::stored_data {
    Message::Message(const network::TransmissionUnit &_unit) {
        if (_unit.GetHeader().data_type != network::DataType::kMessage) {
            // throw
        }

        Deserialize(_unit.GetData());
    }

    std::pair<std::shared_ptr<char[]>, size_t> Message::Serialize() const {
        std::shared_ptr<char[]> result;

        size_t data_size = 0;

        data_size += msg.size() + 1;
        data_size += sender.size() + 1;

        result.reset(new char[data_size]);

        char *buffer = result.get();

        std::strcpy(buffer, msg.c_str());
        buffer += msg.size() + 1;
        std::strcpy(buffer, sender.c_str());

        return { result, data_size };
    }

    Message &Message::Deserialize(const std::shared_ptr<char[]> &_buffer) {
        if (_buffer == nullptr) {
            return *this;
        }

        const char *buffer_ptr = _buffer.get();

        msg = std::string(buffer_ptr);
        buffer_ptr += msg.size() + 1;
        sender = std::string(buffer_ptr);

        return *this;
    }
} // namespace cloud_storage::stored_data
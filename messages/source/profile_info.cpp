#include "profile_info.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::messages {
    ProfileInfoResponse::ProfileInfoResponse(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer ProfileInfoResponse::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kProfileInfoResponse;

        result.length += username.size() + 1;
        result.length += sizeof(max_storage);
        result.length += sizeof(used_storage);

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, username.c_str());
        buffer_ptr += username.size() + 1;

        *reinterpret_cast<decltype(max_storage) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(max_storage);
        buffer_ptr += sizeof(max_storage);

        *reinterpret_cast<decltype(used_storage) *>(buffer_ptr) =
            service::ByteOrderSwitcher::Switch(used_storage);

        return result;
    }

    ProfileInfoResponse &ProfileInfoResponse::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kProfileInfoResponse) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        username = std::string(buffer_ptr);
        buffer_ptr += username.size() + 1;

        max_storage = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(max_storage) *>(buffer_ptr));
        buffer_ptr += sizeof(max_storage);

        used_storage = service::ByteOrderSwitcher::Switch(
            *reinterpret_cast<const decltype(used_storage) *>(buffer_ptr));

        return *this;
    }

    ProfileInfoRequest::ProfileInfoRequest(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer ProfileInfoRequest::Serialize() const {
        network::NetworkBuffer result;

        result.message_id = network::MessageID::kProfileInfoRequest;

        result.length = username.size() + 1;

        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, username.c_str());

        return result;
    }

    ProfileInfoRequest &ProfileInfoRequest::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.message_id != network::MessageID::kProfileInfoRequest) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        username = std::string(buffer_ptr);

        return *this;
    }
} // namespace cloud_storage::messages
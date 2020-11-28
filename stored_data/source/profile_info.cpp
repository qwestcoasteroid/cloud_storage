#include "profile_info.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::stored_data {
    ProfileInfo::ProfileInfo(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer ProfileInfo::Serialize() const {
        network::NetworkBuffer result;

        result.data_type = network::DataType::kProfileInfo;

        result.length = 0;

        result.length += username.size() + 1;
        result.length += sizeof(max_storage);
        result.length += sizeof(used_storage);

        result.buffer.reset(new char[result.length]);

        char *buffer = result.buffer.get();

        std::strcpy(buffer, username.c_str());
        buffer += username.size() + 1;
        *reinterpret_cast<decltype(max_storage) *>(buffer) =
            service::htonll(max_storage);
        buffer += sizeof(max_storage);
        *reinterpret_cast<decltype(used_storage) *>(buffer) =
            service::htonll(used_storage);

        return result;
    }

    ProfileInfo &ProfileInfo::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.data_type != network::DataType::kProfileInfo) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

        username = std::string(buffer_ptr);
        buffer_ptr += username.size() + 1;
        max_storage = service::ntohll(
            *reinterpret_cast<const decltype(max_storage) *>(buffer_ptr));
        buffer_ptr += sizeof(max_storage);
        used_storage = service::ntohll(
            *reinterpret_cast<const decltype(used_storage) *>(buffer_ptr));

        return *this;
    }
} // namespace cloud_storage::stored_data
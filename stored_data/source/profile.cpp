#include "profile.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::stored_data {
    Profile::Profile(const network::TransmissionUnit &unit) {
        if (unit.header.data_type != network::DataType::kProfile) {
            // throw
        }

        Deserialize(unit.data.get());
    }

    network::TransmissionUnit Profile::Serialize() const {
        network::TransmissionUnit result;

        result.header.data_type = network::DataType::kProfile;

        result.header.data_length = 0;

        result.header.data_length += username.size() + 1;
        result.header.data_length += sizeof(max_storage);
        result.header.data_length += sizeof(used_storage);

        result.data.reset(new char[result.header.data_length]);

        char *buffer = result.data.get();

        std::strcpy(buffer, username.c_str());
        buffer += username.size() + 1;
        *reinterpret_cast<decltype(max_storage) *>(buffer) =
            utility::htonll(max_storage);
        buffer += sizeof(max_storage);
        *reinterpret_cast<decltype(used_storage) *>(buffer) =
            utility::htonll(used_storage);

        return result;
    }

    Profile &Profile::Deserialize(const void *buffer) {
        if (buffer == nullptr) {
            return *this;
        }

        const char *buffer_ptr = reinterpret_cast<const char *>(buffer);

        username = std::string(buffer_ptr);
        buffer_ptr += username.size() + 1;
        max_storage = utility::ntohll(
            *reinterpret_cast<const decltype(max_storage) *>(buffer_ptr));
        buffer_ptr += sizeof(max_storage);
        used_storage = utility::ntohll(
            *reinterpret_cast<const decltype(used_storage) *>(buffer_ptr));

        return *this;
    }
} // namespace cloud_storage::stored_data
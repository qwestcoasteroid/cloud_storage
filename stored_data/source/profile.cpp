#include "profile.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::stored_data {
    Profile::Profile(const network::TransmissionUnit &unit) {
        if (unit.GetHeader().data_type != network::DataType::kProfile) {
            // throw
        }

        Deserialize(unit.GetData());
    }

    std::pair<std::shared_ptr<char>, size_t> Profile::Serialize() const {
        std::shared_ptr<char> result;

        size_t data_size = 0;

        data_size += username.size() + 1;
        data_size += sizeof(max_storage);
        data_size += sizeof(used_storage);

        result.reset(new char[data_size]);

        char *buffer = result.get();

        std::strcpy(buffer, username.c_str());
        buffer += username.size() + 1;
        *reinterpret_cast<decltype(max_storage) *>(buffer) =
            service::htonll(max_storage);
        buffer += sizeof(max_storage);
        *reinterpret_cast<decltype(used_storage) *>(buffer) =
            service::htonll(used_storage);

        return { result, data_size };
    }

    Profile &Profile::Deserialize(const std::shared_ptr<char> buffer) {
        if (buffer == nullptr) {
            return *this;
        }

        const char *buffer_ptr = buffer.get();

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
#include "profile.hpp"

#include <cstring>

#include "help_func.hpp"

namespace cloud_storage::stored_data {
    void Profile::Serialize() {
        header.data_length = 0;

        header.data_length += username.size() + 1;
        header.data_length += sizeof(max_storage);
        header.data_length += sizeof(used_storage);

        data.reset(new char[header.data_length]);

        char *buffer = reinterpret_cast<char *>(data.get());

        std::strcpy(buffer, username.c_str());
        buffer += username.size() + 1;
        *reinterpret_cast<uint64_t *>(buffer) = utility::htonll(max_storage);
        buffer += sizeof(max_storage);
        *reinterpret_cast<uint64_t *>(buffer) = utility::htonll(used_storage);
    }

    void Profile::Deserialize(const void *bytes) {
        const char *buffer = reinterpret_cast<const char *>(bytes);

        username = std::string(buffer);
        buffer += username.size() + 1;
        max_storage = utility::ntohll(*reinterpret_cast<const uint64_t *>(buffer));
        buffer += sizeof(max_storage);
        used_storage = utility::ntohll(*reinterpret_cast<const uint64_t *>(buffer));
    }
}
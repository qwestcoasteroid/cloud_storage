#ifndef CLOUD_STORAGE_STORED_DATA_PROFILE_HPP_
#define CLOUD_STORAGE_STORED_DATA_PROFILE_HPP_

#include <string>

#include "serializable.hpp"
#include "protocol.hpp"

// TODO: Define valid fields

namespace cloud_storage::stored_data {
    class ProfileInfo : public service::Serializable {
    public:
        inline ProfileInfo() noexcept {}
        ProfileInfo(const network::Packet &_packet);

        std::pair<std::shared_ptr<char[]>, size_t> Serialize() const override;
        ProfileInfo &Deserialize(const std::shared_ptr<char[]> &_buffer) override;

        std::string username;
        uint64_t max_storage{};
        uint64_t used_storage{};
    };
} // namespace cloud_storage::stored_data

#endif // CLOUD_STORAGE_STORED_DATA_PROFILE_HPP_
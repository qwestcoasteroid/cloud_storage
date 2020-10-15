#ifndef CLOUD_STORAGE_STORED_DATA_PROFILE_HPP_
#define CLOUD_STORAGE_STORED_DATA_PROFILE_HPP_

#include <string>

#include "serializable.hpp"
#include "protocol.hpp"

// TODO: Define valid fields

namespace cloud_storage::stored_data {
    class Profile : public utility::Serializable, public network::TransmissionUnit {
    public:
        Profile() { header.data_type = network::DataType::kProfile; }

        void Serialize() const override;
        void Deserialize(const void *) override;

        std::string username;
        uint64_t max_storage{};
        uint64_t used_storage{};
    };
} // namespace cloud_storage::stored_data

#endif // CLOUD_STORAGE_STORED_DATA_PROFILE_HPP_
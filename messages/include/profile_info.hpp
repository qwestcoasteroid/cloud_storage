#ifndef CLOUD_STORAGE_MESSAGES_PROFILE_INFO_HPP_
#define CLOUD_STORAGE_MESSAGES_PROFILE_INFO_HPP_

#include <cstdint>
#include <string>

#include "serializable.hpp"
#include "protocol.hpp"

namespace cloud_storage::messages {
    class ProfileInfoResponse : public service::Serializable {
    public:
        inline ProfileInfoResponse() noexcept {}
        ProfileInfoResponse(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        ProfileInfoResponse &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string username;
        uint64_t max_storage{};
        uint64_t used_storage{};
    };

    class ProfileInfoRequest : public service::Serializable {
    public:
        inline ProfileInfoRequest() noexcept {}
        ProfileInfoRequest(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        ProfileInfoRequest &Deserialize(const network::NetworkBuffer &_buffer) override;

        std::string username;
    };
} // namespace cloud_storage::messages

#endif // CLOUD_STORAGE_MESSAGES_PROFILE_INFO_HPP_
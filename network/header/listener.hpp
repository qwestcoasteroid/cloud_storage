#ifndef CLOUD_STORAGE_NETWORK_SERVER_HPP_
#define CLOUD_STORAGE_NETWORK_SERVER_HPP_

#include <string_view>

#include "network_object.hpp"
#include "connection.hpp"

namespace cloud_storage::network {
    class Listener : public NetworkObject {
    public:
        explicit Listener(std::string_view _port);

        ~Listener();

        Connection Accept() const;
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_SERVER_HPP_
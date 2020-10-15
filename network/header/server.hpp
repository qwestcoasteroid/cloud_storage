#ifndef CLOUD_STORAGE_NETWORK_SERVER_HPP_
#define CLOUD_STORAGE_NETWORK_SERVER_HPP_

#include <string_view>

#include "network_object.hpp"
#include "client.hpp"

namespace cloud_storage::network {
    class Server : public NetworkObject {
    public:
        explicit Server(std::string_view port);

        ~Server();

        Client Accept() const;
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_SERVER_HPP_
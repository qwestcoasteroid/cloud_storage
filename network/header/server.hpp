#ifndef CLOUD_STORAGE_NETWORK_SERVER_HPP_
#define CLOUD_STORAGE_NETWORK_SERVER_HPP_

#include <string_view>

#include "network_object.hpp"
#include "connection_info.hpp"
#include "client.hpp"

namespace cloud_storage::network {
    class Server : public NetworkObject {
    public:
        Server(std::string_view port);

        ~Server();

        const ConnectionInfo &GetConnectionInfo() const;

        Client Accept() const;

    private:
        ConnectionInfo connection_info_; // Redundant...
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_SERVER_HPP_
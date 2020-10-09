#ifndef CLOUD_STORAGE_NETWORK_CLIENT_HPP_
#define CLOUD_STORAGE_NETWORK_CLIENT_HPP_

#include <string_view>

#include "network_object.hpp"
#include "connection_info.hpp"

namespace cloud_storage::network {
    class Client : public NetworkObject {
    public:
        Client(std::string_view ip_address, std::string_view port);

        Client(Client &&client) noexcept;
        Client& operator=(Client &&client) noexcept;

        ~Client();

        bool Connect() const;

        const ConnectionInfo &GetConnectionInfo() const;

        friend class Server;

    private:
        Client();

        ConnectionInfo connection_info_;
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_CLIENT_HPP_
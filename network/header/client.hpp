#ifndef CLOUD_STORAGE_NETWORK_CLIENT_HPP_
#define CLOUD_STORAGE_NETWORK_CLIENT_HPP_

#include <string_view>

#include "network_object.hpp"

namespace cloud_storage::network {
    class Client : public NetworkObject {
    public:
        Client(std::string_view _ip_address, std::string_view _port);

        Client(Client &&_client) noexcept;
        Client& operator=(Client &&_client) noexcept;

        ~Client();

        void Connect() const;
        void Disconnect() const;

        inline bool operator==(const Client &_object) const noexcept;
        inline bool operator!=(const Client &_object) const noexcept;

        friend class Server;

    private:
        inline Client() noexcept {};
    };

    bool Client::operator==(const Client &_object) const noexcept {
        return socket_info_.socket == _object.socket_info_.socket;
    }

    bool Client::operator!=(const Client &_object) const noexcept {
        return !(*this == _object);
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_CLIENT_HPP_
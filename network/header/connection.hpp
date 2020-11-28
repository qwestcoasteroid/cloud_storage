#ifndef CLOUD_STORAGE_NETWORK_CLIENT_HPP_
#define CLOUD_STORAGE_NETWORK_CLIENT_HPP_

#include <string_view>

#include "network_object.hpp"

namespace cloud_storage::network {
    class Connection : public NetworkObject {
    public:
        inline Connection() noexcept {};
        Connection(std::string_view _ip_address, std::string_view _port);

        Connection(Connection &&_client) noexcept;
        Connection& operator=(Connection &&_client) noexcept;

        ~Connection();

        void Connect() const;
        void Disconnect() const;

        inline bool operator==(const Connection &_object) const noexcept;
        inline bool operator!=(const Connection &_object) const noexcept;

        friend class Listener;   
    };

    bool Connection::operator==(const Connection &_object) const noexcept {
        return socket_info_.socket == _object.socket_info_.socket;
    }

    bool Connection::operator!=(const Connection &_object) const noexcept {
        return !(*this == _object);
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_CLIENT_HPP_
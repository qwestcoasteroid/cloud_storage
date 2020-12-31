#ifndef CLOUD_STORAGE_NETWORK_CLIENT_HPP_
#define CLOUD_STORAGE_NETWORK_CLIENT_HPP_

#include <string_view>
#include <memory>

#include "network_object.hpp"
#include "protocol.hpp"

namespace cloud_storage::network {
    class Connection : public NetworkObject {
    public:
        inline Connection() noexcept {};
        Connection(std::string_view _ip_address, std::string_view _port);

        Connection(Connection &&_other) noexcept;
        Connection& operator=(Connection &&_other) noexcept;

        ~Connection();

        void Connect() const;
        void Disconnect() const;

        inline bool operator==(const Connection &_other) const noexcept;
        inline bool operator!=(const Connection &_other) const noexcept;

        friend class Listener;
    };

    bool Connection::operator==(const Connection &_other) const noexcept {
        return socket_info_.socket == _other.socket_info_.socket;
    }

    bool Connection::operator!=(const Connection &_other) const noexcept {
        return !(*this == _other);
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_CLIENT_HPP_
#include "client.hpp"

namespace cloud_storage::network {
    Client::Client() {}

    Client::Client(std::string_view ip_address, std::string_view port) {
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));

        hints.ai_socktype = SOCK_STREAM;

        addrinfo *peer_address;

        if (getaddrinfo(ip_address.data(), port.data(), &hints, &peer_address)) {
            // throw
        }

        connection_info_.socket_ = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);

        if (connection_info_.socket_ == INVALID_SOCKET) {
            // throw
        }

        CopyMemory(&connection_info_.address_, peer_address->ai_addr,
            peer_address->ai_addrlen);

        connection_info_.address_length_ = peer_address->ai_addrlen;

        freeaddrinfo(peer_address);
    }

    Client::Client(Client &&client) noexcept {
        CopyMemory(&connection_info_, &client.connection_info_,
            sizeof(connection_info_));

        client.connection_info_.socket_ = INVALID_SOCKET;
    }

    Client& Client::operator=(Client &&client) noexcept {
        if (this == &client) {
            return *this;
        }

        CopyMemory(&connection_info_, &client.connection_info_,
            sizeof(connection_info_));

        client.connection_info_.socket_ = INVALID_SOCKET;

        return *this;
    }

    Client::~Client() {
        if (connection_info_.socket_ != INVALID_SOCKET) {
            closesocket(connection_info_.socket_);
        }
    }

    const ConnectionInfo &Client::GetConnectionInfo() const {
        return connection_info_;
    }

    bool Client::Connect() const {
        if (connect(connection_info_.socket_,
            reinterpret_cast<const sockaddr *>(&connection_info_.address_),
            connection_info_.address_length_)) {
            // throw
            return false;
        }

        return true;
    }
}; // namespace cloud_storage::network
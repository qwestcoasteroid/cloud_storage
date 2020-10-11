#include "client.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif

namespace cloud_storage::network {
    Client::Client() {}

    Client::Client(std::string_view ip_address, std::string_view port) {
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));

        hints.ai_socktype = SOCK_STREAM;

        addrinfo *peer_address;

        if (getaddrinfo(ip_address.data(), port.data(), &hints, &peer_address)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling getaddrinfo() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        connection_info_.socket = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);

        if (connection_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling socket() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        CopyMemory(&connection_info_.address, peer_address->ai_addr,
            peer_address->ai_addrlen);

        connection_info_.address_length = peer_address->ai_addrlen;

        freeaddrinfo(peer_address);
    }

    Client::Client(Client &&client) noexcept {
        CopyMemory(&connection_info_, &client.connection_info_,
            sizeof(connection_info_));

        client.connection_info_.socket = INVALID_SOCKET;
    }

    Client& Client::operator=(Client &&client) noexcept {
        if (this == &client) {
            return *this;
        }

        CopyMemory(&connection_info_, &client.connection_info_,
            sizeof(connection_info_));

        client.connection_info_.socket = INVALID_SOCKET;

        return *this;
    }

    Client::~Client() {
        if (connection_info_.socket != INVALID_SOCKET) {
            closesocket(connection_info_.socket);
        }
    }

    const ConnectionInfo &Client::GetConnectionInfo() const {
        return connection_info_;
    }

    bool Client::Connect() const {
        if (connect(connection_info_.socket,
            reinterpret_cast<const sockaddr *>(&connection_info_.address),
            connection_info_.address_length)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling connect() (" << WSAGetLastError() << ")\n";
#endif
            // throw
            return false;
        }

        return true;
    }
}; // namespace cloud_storage::network
#include "client.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

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
#endif // DEBUG_OUTPUT
            // throw
        }

        socket_info_.socket = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);

        if (socket_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling socket() (" << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        CopyMemory(&socket_info_.address, peer_address->ai_addr,
            peer_address->ai_addrlen);

        socket_info_.address_length = peer_address->ai_addrlen;
        socket_info_.socket_type = peer_address->ai_socktype;

        freeaddrinfo(peer_address);
    }

    Client::Client(Client &&client) noexcept {
        CopyMemory(&socket_info_, &client.socket_info_,
            sizeof(socket_info_));

        client.socket_info_.socket = INVALID_SOCKET;
    }

    Client& Client::operator=(Client &&client) noexcept {
        if (this == &client) {
            return *this;
        }

        CopyMemory(&socket_info_, &client.socket_info_,
            sizeof(socket_info_));

        client.socket_info_.socket = INVALID_SOCKET;

        return *this;
    }

    Client::~Client() {
        if (socket_info_.socket != INVALID_SOCKET) {
            closesocket(socket_info_.socket);
        }
    }

    bool Client::Connect() const {
        if (connect(socket_info_.socket,
            reinterpret_cast<const sockaddr *>(&socket_info_.address),
            socket_info_.address_length)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling connect() (" << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
            return false;
        }

        return true;
    }
}; // namespace cloud_storage::network
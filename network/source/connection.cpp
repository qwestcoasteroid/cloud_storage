#include "connection.hpp"

#include <cstring>

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "exceptions.hpp"

static constexpr uint16_t kReceiveBufferSize = (1 << 10) * 8;
static constexpr uint16_t kSendBufferSize    = (1 << 10) * 8;

namespace cloud_storage::network {
    Connection::Connection(std::string_view _ip_address, std::string_view _port) {
        addrinfo hints;
        std::memset(&hints, 0, sizeof(hints));

        hints.ai_socktype = SOCK_STREAM;

        addrinfo *peer_address = NULL;

        if (getaddrinfo(_ip_address.data(), _port.data(), &hints, &peer_address)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling getaddrinfo() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            throw exceptions::SocketError(WSAGetLastError());
        }

        socket_info_.socket = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);

        if (socket_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling socket() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            throw exceptions::SocketError(WSAGetLastError());
        }

        setsockopt(socket_info_.socket, SOL_SOCKET, SO_RCVBUF,
            reinterpret_cast<const char *>(&kReceiveBufferSize),
            sizeof(kReceiveBufferSize)
        );

        setsockopt(socket_info_.socket, SOL_SOCKET, SO_SNDBUF,
            reinterpret_cast<const char *>(&kSendBufferSize),
            sizeof(kSendBufferSize)
        );

        std::memcpy(&socket_info_.address, peer_address->ai_addr,
            peer_address->ai_addrlen);

        socket_info_.address_length = peer_address->ai_addrlen;
        socket_info_.socket_type = peer_address->ai_socktype;

        freeaddrinfo(peer_address);
    }

    Connection::Connection(Connection &&_other) noexcept {

        std::memcpy(&socket_info_, &_other.socket_info_,
            sizeof(socket_info_));

        _other.socket_info_.socket = INVALID_SOCKET;
    }

    Connection& Connection::operator=(Connection &&_other) noexcept {
        if (this == &_other) {
            return *this;
        }

        std::memcpy(&socket_info_, &_other.socket_info_,
            sizeof(socket_info_));

        _other.socket_info_.socket = INVALID_SOCKET;

        return *this;
    }

    Connection::~Connection() {
        if (socket_info_.socket != INVALID_SOCKET) {
            closesocket(socket_info_.socket);
        }
    }

    void Connection::Connect() const {
        if (connect(socket_info_.socket,
            reinterpret_cast<const sockaddr *>(&socket_info_.address),
            socket_info_.address_length)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling connect() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            throw exceptions::SocketError(WSAGetLastError());
        }
    }

    void Connection::Disconnect() const {
        if (shutdown(socket_info_.socket, SD_SEND)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling shutdown() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            throw exceptions::SocketError(WSAGetLastError());
        }
    }
}; // namespace cloud_storage::network
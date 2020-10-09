#include "server.hpp"

#define QUEUE_SIZE 16

namespace cloud_storage::network {
    Server::Server(std::string_view port) {
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));

        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_INET6;
        hints.ai_flags = AI_PASSIVE;

        addrinfo *bind_address;

        if (getaddrinfo(nullptr, port.data(), &hints, &bind_address)) {
            // throw
        }

        connection_info_.socket_ = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);

        if (connection_info_.socket_ == INVALID_SOCKET) {
            // throw
        }

        int option = 0;

        if (setsockopt(connection_info_.socket_, IPPROTO_IPV6, IPV6_V6ONLY,
            reinterpret_cast<const char *>(&option), sizeof(option))) {
            // throw
        }

        if (bind(connection_info_.socket_, bind_address->ai_addr,
            bind_address->ai_addrlen)) {
            // throw
        }

        if (listen(connection_info_.socket_, QUEUE_SIZE) < 0) {
            // throw
        }

        CopyMemory(&connection_info_.address_, bind_address->ai_addr,
            bind_address->ai_addrlen);

        connection_info_.address_length_ = bind_address->ai_addrlen;

        freeaddrinfo(bind_address);
    }

    Server::~Server() {
        if (connection_info_.socket_ != INVALID_SOCKET) {
            closesocket(connection_info_.socket_);
        }
    }

    const ConnectionInfo &Server::GetConnectionInfo() const {
        return connection_info_;
    }

    Client Server::Accept() const {
        Client new_client;

        new_client.connection_info_.socket_ = accept(connection_info_.socket_,
            reinterpret_cast<sockaddr *>(&new_client.connection_info_.address_),
            &new_client.connection_info_.address_length_);

        if (new_client.connection_info_.socket_ == INVALID_SOCKET) {
            // throw
        }

        return new_client;
    }
}; // namespace cloud_storage::network
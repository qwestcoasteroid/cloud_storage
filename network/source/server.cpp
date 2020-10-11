#include "server.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif

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
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling getaddrinfo() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        connection_info_.socket = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);

        if (connection_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling socket() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        int option = 0;

        if (setsockopt(connection_info_.socket, IPPROTO_IPV6, IPV6_V6ONLY,
            reinterpret_cast<const char *>(&option), sizeof(option))) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling setsockopt() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        if (bind(connection_info_.socket, bind_address->ai_addr,
            bind_address->ai_addrlen)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling bind() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        if (listen(connection_info_.socket, QUEUE_SIZE) < 0) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling listen() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        CopyMemory(&connection_info_.address, bind_address->ai_addr,
            bind_address->ai_addrlen);

        connection_info_.address_length = bind_address->ai_addrlen;

        freeaddrinfo(bind_address);
    }

    Server::~Server() {
        if (connection_info_.socket != INVALID_SOCKET) {
            closesocket(connection_info_.socket);
        }
    }

    const ConnectionInfo &Server::GetConnectionInfo() const {
        return connection_info_;
    }

    Client Server::Accept() const {
        Client new_client;

        new_client.connection_info_.socket = accept(connection_info_.socket,
            reinterpret_cast<sockaddr *>(&new_client.connection_info_.address),
            &new_client.connection_info_.address_length);

        if (new_client.connection_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling accept() (" << WSAGetLastError() << ")\n";
#endif
            // throw
        }

        return new_client;
    }
}; // namespace cloud_storage::network
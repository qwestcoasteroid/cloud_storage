#include "server.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#define QUEUE_SIZE 16

namespace cloud_storage::network {
    Server::Server(std::string_view _port) {
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));

        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_INET6;
        hints.ai_flags = AI_PASSIVE;

        addrinfo *bind_address;

        if (getaddrinfo(nullptr, _port.data(), &hints, &bind_address)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling getaddrinfo() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        socket_info_.socket = socket(bind_address->ai_family,
            bind_address->ai_socktype, bind_address->ai_protocol);

        if (socket_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling socket() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        int option = 0;

        if (setsockopt(socket_info_.socket, IPPROTO_IPV6, IPV6_V6ONLY,
            reinterpret_cast<const char *>(&option), sizeof(option))) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling setsockopt() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        if (bind(socket_info_.socket, bind_address->ai_addr,
            bind_address->ai_addrlen)) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling bind() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        if (listen(socket_info_.socket, QUEUE_SIZE) < 0) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling listen() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        CopyMemory(&socket_info_.address, bind_address->ai_addr,
            bind_address->ai_addrlen);

        socket_info_.address_length = bind_address->ai_addrlen;
        socket_info_.socket_type = bind_address->ai_socktype;

        freeaddrinfo(bind_address);
    }

    Server::~Server() {
        if (socket_info_.socket != INVALID_SOCKET) {
            closesocket(socket_info_.socket);
        }
    }

    Client Server::Accept() const {
        Client new_client;

        new_client.socket_info_.socket = accept(socket_info_.socket,
            reinterpret_cast<sockaddr *>(&new_client.socket_info_.address),
            &new_client.socket_info_.address_length);

        if (new_client.socket_info_.socket == INVALID_SOCKET) {
#ifdef DEBUG_OUTPUT
            std::cerr << "Error calling accept() ("
                << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
            // throw
        }

        new_client.socket_info_.socket_type = socket_info_.socket_type;

        return new_client;
    }
} // namespace cloud_storage::network
#include "server.hpp"

#define QUEUE_SIZE 16

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

    m_connection_info.m_socket = socket(bind_address->ai_family,
        bind_address->ai_socktype, bind_address->ai_protocol);

    if (m_connection_info.m_socket == INVALID_SOCKET) {
        // throw
    }

    int option = 0;

    if (setsockopt(m_connection_info.m_socket, IPPROTO_IPV6, IPV6_V6ONLY,
        reinterpret_cast<const char *>(&option), sizeof(option))) {
        // throw
    }

    if (bind(m_connection_info.m_socket, bind_address->ai_addr,
        bind_address->ai_addrlen)) {
        // throw
    }

    if (listen(m_connection_info.m_socket, QUEUE_SIZE) < 0) {
        // throw
    }

    CopyMemory(&m_connection_info.m_address, bind_address->ai_addr,
        bind_address->ai_addrlen);

    m_connection_info.m_address_length = bind_address->ai_addrlen;

    freeaddrinfo(bind_address);
}

Server::~Server() {
    if (m_connection_info.m_socket != INVALID_SOCKET) {
        closesocket(m_connection_info.m_socket);
    }
}

const ConnectionInfo &Server::GetConnectionInfo() const {
    return m_connection_info;
}

Client Server::Accept() const {
    Client new_client;

    sockaddr_storage client_address;
    socklen_t client_length = sizeof(client_address);

    new_client.m_connection_info.m_socket = accept(m_connection_info.m_socket,
        reinterpret_cast<sockaddr *>(&client_address), &client_length);

    if (new_client.m_connection_info.m_socket == INVALID_SOCKET) {
        // throw
    }

    CopyMemory(&new_client.m_connection_info.m_address,
        &client_address, client_length);

    new_client.m_connection_info.m_address_length = client_length;

    return new_client;
}
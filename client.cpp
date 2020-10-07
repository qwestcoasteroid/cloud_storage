#include "client.hpp"

Client::Client() {}

Client::Client(std::string_view ip_address, std::string_view port) {
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_socktype = SOCK_STREAM;

    addrinfo *peer_address;

    if (getaddrinfo(ip_address.data(), port.data(), &hints, &peer_address)) {
        // throw
    }

    m_connection_info.m_socket = socket(peer_address->ai_family,
        peer_address->ai_socktype, peer_address->ai_protocol);

    if (m_connection_info.m_socket == INVALID_SOCKET) {
        // throw
    }

    CopyMemory(&m_connection_info.m_address, peer_address->ai_addr,
        peer_address->ai_addrlen);

    m_connection_info.m_address_length = peer_address->ai_addrlen;

    freeaddrinfo(peer_address);
}

Client::Client(Client &&client) noexcept {
    CopyMemory(&m_connection_info, &client.m_connection_info,
        sizeof(m_connection_info));

    client.m_connection_info.m_socket = INVALID_SOCKET;
}

Client& Client::operator=(Client &&client) noexcept {
    if (this == &client) {
        return *this;
    }

    CopyMemory(&m_connection_info, &client.m_connection_info,
        sizeof(m_connection_info));

    client.m_connection_info.m_socket = INVALID_SOCKET;

    return *this;
}

Client::~Client() {
    if (m_connection_info.m_socket != INVALID_SOCKET) {
        closesocket(m_connection_info.m_socket);
    }
}

const ConnectionInfo &Client::GetConnectionInfo() const {
    return m_connection_info;
}

bool Client::Connect() const {
    if (connect(m_connection_info.m_socket,
        reinterpret_cast<const sockaddr *>(&m_connection_info.m_address),
        m_connection_info.m_address_length)) {
        // throw
        return false;
    }

    return true;
}
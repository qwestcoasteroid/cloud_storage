#ifndef CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_
#define CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_

#include "../../utility/header/targetver.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>

namespace cloud_storage::network {
    struct ConnectionInfo {
        sockaddr_storage address_{ 0 };
        socklen_t address_length_{ 0 };
        SOCKET socket_{ INVALID_SOCKET };
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_
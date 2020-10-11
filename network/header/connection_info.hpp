#ifndef CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_
#define CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_

#include "targetver.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>

namespace cloud_storage::network {
    struct ConnectionInfo {
        sockaddr_storage address{ 0 };
        socklen_t address_length{ sizeof(address) };
        SOCKET socket{ INVALID_SOCKET };
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_
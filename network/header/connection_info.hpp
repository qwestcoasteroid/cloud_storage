#ifndef CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_
#define CLOUD_STORAGE_NETWORK_CONNECTION_INFO_HPP_

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#ifndef IPV6_V6ONLY
#define IPV6_V6ONLY 27
#endif

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
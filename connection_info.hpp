#ifndef CLOUD_STORAGE_CONNECTION_INFO_HPP_
#define CLOUD_STORAGE_CONNECTION_INFO_HPP_

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#ifndef IPV6_V6ONLY
#define IPV6_V6ONLY 27
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

struct ConnectionInfo {
    sockaddr_storage m_address{ 0 };
    socklen_t m_address_length{ 0 };
    SOCKET m_socket{ INVALID_SOCKET };
};

#endif // CLOUD_STORAGE_CONNECTION_INFO_HPP_
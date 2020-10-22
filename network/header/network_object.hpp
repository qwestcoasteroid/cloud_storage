#ifndef CLOUD_STORAGE_NETWORK_NETWORK_OBJECT_HPP
#define CLOUD_STORAGE_NETWORK_NETWORK_OBJECT_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

namespace cloud_storage::network {
    struct SocketInfo {
        sockaddr_storage address{ 0 };
        socklen_t address_length{ sizeof(address) };
        SOCKET socket{ INVALID_SOCKET };
        int socket_type{ ~0 };
    };

    class NetworkObject {
    public:
        NetworkObject();
        virtual ~NetworkObject() = 0;

        inline const SocketInfo &GetSocketInfo() const {
            return socket_info_;
        }

    protected:
        SocketInfo socket_info_;

    private:
        static inline unsigned long count_{ 0 };
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_NETWORK_OBJECT_HPP
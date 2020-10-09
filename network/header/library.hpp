#ifndef CLOUD_STORAGE_NETWORK_LIBRARY_HPP_
#define CLOUD_STORAGE_NETWORK_LIBRARY_HPP_

#include <winsock2.h>

namespace cloud_storage::network {
    class Library {
    public:
        struct WSAVersion {
            WORD major : 8;
            WORD minor : 8;
        };

        static void Initialize();
        static void Deinitialize();

    private:
        static inline WSADATA wsa_{ 0 };
        static inline WSAVersion version_{ 0 };
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_LIBRARY_HPP_
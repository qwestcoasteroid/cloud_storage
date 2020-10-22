#ifndef CLOUD_STORAGE_NETWORK_LIBRARY_HPP_
#define CLOUD_STORAGE_NETWORK_LIBRARY_HPP_

#include "targetver.hpp"

#include <winsock2.h>

namespace cloud_storage::network {
    class Library {
    public:
        Library() = delete;

        static void Initialize();
        static void Deinitialize();

    private:
        static inline WSADATA wsa_{ 0 };
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_LIBRARY_HPP_
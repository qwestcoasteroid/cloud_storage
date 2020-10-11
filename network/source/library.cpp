#include "library.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif

#pragma comment(lib, "ws2_32.lib")

#define EXPECTED_VERSION_MAJOR 2
#define EXPECTED_VERSION_MINOR 2

namespace cloud_storage::network {
    void Library::Initialize() {
        int result = WSAStartup(MAKEWORD(EXPECTED_VERSION_MAJOR,
            EXPECTED_VERSION_MINOR), &wsa_);

        switch (result) {
        case WSAVERNOTSUPPORTED:
            version_.minor = static_cast<unsigned char>(wsa_.wHighVersion >> 8);
            version_.major = static_cast<unsigned char>(wsa_.wHighVersion);
            WSAStartup(MAKEWORD(version_.major, version_.minor), &wsa_);
            if (version_.major != EXPECTED_VERSION_MAJOR) {
                WSACleanup();
#ifdef DEBUG_OUTPUT
                std::cerr << "Expected version of WSA ("
                    << EXPECTED_VERSION_MAJOR << "."
                    << EXPECTED_VERSION_MINOR << ") isn't supported!\n";
#endif
                // throw
            }
            break;
        case WSASYSNOTREADY:
#ifdef DEBUG_OUTPUT
            std::cerr << "Network subsystem isn't ready!\n";
#endif
            // throw
            break;
        default:
            version_.minor = static_cast<unsigned char>(wsa_.wVersion >> 8);
            version_.major = static_cast<unsigned char>(wsa_.wVersion);
            break;
        }
    }

    void Library::Deinitialize() {
        WSACleanup();
    }
} // namespace cloud_storage::network
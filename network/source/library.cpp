#include "library.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#pragma comment(lib, "ws2_32.lib")

#define EXPECTED_WSA_VERSION_MINOR 2
#define EXPECTED_WSA_VERSION_MAJOR 2

namespace cloud_storage::network {
    void Library::Initialize() {
        int result = WSAStartup(MAKEWORD(EXPECTED_WSA_VERSION_MAJOR,
            EXPECTED_WSA_VERSION_MINOR), &wsa_);

        switch (result) {
        case WSAVERNOTSUPPORTED:
            WSAStartup(MAKEWORD(LOBYTE(wsa_.wHighVersion),
                HIBYTE(wsa_.wHighVersion)), &wsa_);
            if (LOBYTE(wsa_.wHighVersion) != EXPECTED_WSA_VERSION_MAJOR ||
                HIBYTE(wsa_.wHighVersion) != EXPECTED_WSA_VERSION_MINOR) {
                WSACleanup();
#ifdef DEBUG_OUTPUT
                std::cerr << "Expected version of WSA ("
                    << EXPECTED_WSA_VERSION_MAJOR << "."
                    << EXPECTED_WSA_VERSION_MINOR << ") isn't supported!\n";
#endif // DEBUG_OUTPUT
                // throw
            }
            break;
        case WSASYSNOTREADY:
#ifdef DEBUG_OUTPUT
            std::cerr << "Network subsystem isn't ready!\n";
#endif // DEBUG_OUTPUT
            // throw
            break;
        default:
            break;
        }
    }

    void Library::Deinitialize() {
        WSACleanup();
    }
} // namespace cloud_storage::network
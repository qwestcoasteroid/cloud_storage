#include <iostream>

#include "client.hpp"

int main() {
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    Client client("127.0.0.1", "43000");

    if (!client.Connect()) {
        std::cout << "Can't connect!\n\n";
    }
    else {
        std::cout << "Connected!\n\n";
    }

    WSACleanup();

    return 0;
}
#include <iostream>

#include "network/header/client.hpp"

int main() {
    cloud_storage::network::Client client("127.0.0.1", "43000");

    if (!client.Connect()) {
        std::cout << "Can't connect!\n\n";
    }
    else {
        std::cout << "Connected!\n\n";
    }

    return 0;
}
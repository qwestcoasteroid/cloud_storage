#include <iostream>

#include "network/header/server.hpp"
#include "network/header/client.hpp"

int main() {
    cloud_storage::network::Server server("43000");

    cloud_storage::network::Client client = server.Accept();

    char ipaddr[100];

    getnameinfo(reinterpret_cast<const sockaddr *>(&client.GetConnectionInfo().address_),
        client.GetConnectionInfo().address_length_, ipaddr,
        sizeof(ipaddr), nullptr, 0, NI_NUMERICHOST);

    std::cout << "Client [" << ipaddr << "] connected!\n\n";

    return 0;
}
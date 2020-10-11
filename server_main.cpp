#include <iostream>

#include "server.hpp"
#include "client.hpp"
#include "profile.hpp"

int main() {
    cloud_storage::network::Server server("43000");

    cloud_storage::network::Client client = server.Accept();

    char ipaddr[100];

    getnameinfo(reinterpret_cast<const sockaddr *>(&client.GetConnectionInfo().address),
        client.GetConnectionInfo().address_length, ipaddr,
        sizeof(ipaddr), nullptr, 0, NI_NUMERICHOST);

    std::cout << "Client [" << ipaddr << "] connected!\n\n";

    cloud_storage::stored_data::Profile prof;

    prof.username = "sakuyamaxanadu";
    prof.max_storage = (1 << 10) * 1024;
    prof.used_storage = (1 << 10) * 512;

    prof.Serialize();

    send(client.GetConnectionInfo().socket, (const char *)prof.data.get(), prof.header.data_length, 0);

    return 0;
}
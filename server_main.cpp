#include <iostream>

#include "server.hpp"
#include "client.hpp"

int main() {
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    Server server("43000");

    Client client = server.Accept();

    char ipaddr[100];

    getnameinfo(reinterpret_cast<const sockaddr *>(&client.GetConnectionInfo().m_address),
        client.GetConnectionInfo().m_address_length, ipaddr,
        sizeof(ipaddr), nullptr, 0, NI_NUMERICHOST);

    std::cout << "Client [" << ipaddr << "] connected!\n\n";

    WSACleanup();

    return 0;
}
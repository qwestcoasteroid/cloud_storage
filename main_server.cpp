#include <windows.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "client.hpp"
#include "server.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "message.hpp"

CRITICAL_SECTION client_section;

std::vector<std::pair<cloud_storage::network::Client *, std::string>> clients;

DWORD WINAPI ClientHandler(void *_client) {
    using cloud_storage::network::Client;
    using cloud_storage::network::TransmissionUnit;
    using cloud_storage::network::NetworkReadingStream;
    using cloud_storage::network::NetworkWritingStream;
    using cloud_storage::stored_data::Message;

    Client &client = *reinterpret_cast<Client *>(_client);

    auto client_info = *std::find_if(clients.cbegin(), clients.cend(),
        [_client](auto i){
            return _client == i.first;
    });

    NetworkReadingStream reader(client);
    NetworkWritingStream writer(client);

    while (true) {
        TransmissionUnit unit;

        try {
            unit = reader.Read();
        }
        catch (...) {
            EnterCriticalSection(&client_section);

            clients.erase(std::find_if(clients.cbegin(), clients.cend(),
                [_client](auto i) {
                    return _client == i.first;
            }));

            LeaveCriticalSection(&client_section);

            client.Disconnect();

            std::cout << client_info.second << " disconnected!\n";

            delete reinterpret_cast<Client *>(_client);

            return 0;
        }

        for (const auto &i : clients) {
            if (i.first != _client) {
                writer.Reset(*i.first);
                writer.Write(unit);
            }
        }

        writer.Reset(client);
    }

    return 0;
}

int main() {
    using namespace cloud_storage;

    InitializeCriticalSection(&client_section);

    std::string port;

    std::cout << "Port: ";
    std::cin >> port;

    network::Server server(port);

    char addr[100];

    while (true) {
        network::Client *client = new network::Client(server.Accept());

        getnameinfo((const sockaddr *)&client->GetSocketInfo().address,
            client->GetSocketInfo().address_length, addr, sizeof(addr),
            NULL, 0, NI_NUMERICHOST);

        std::cout << addr << " connected!" << std::endl;

        EnterCriticalSection(&client_section);

        clients.emplace_back(client, addr);

        LeaveCriticalSection(&client_section);

        QueueUserWorkItem(ClientHandler, client, 0);
    }

    DeleteCriticalSection(&client_section);

    return 0;
}
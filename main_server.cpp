#include <windows.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "connection.hpp"
#include "listener.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "message.hpp"

CRITICAL_SECTION client_section;

std::vector<std::pair<cloud_storage::network::Connection *, std::string>> clients;

DWORD WINAPI ClientHandler(void *_client) {
    using cloud_storage::network::Connection;
    using cloud_storage::network::Packet;
    using cloud_storage::network::NetworkReadingStream;
    using cloud_storage::network::NetworkWritingStream;
    using cloud_storage::stored_data::Message;

    Connection &connection = *reinterpret_cast<Connection *>(_client);

    auto client_info = *std::find_if(clients.cbegin(), clients.cend(),
        [_client](auto i){
            return _client == i.first;
    });

    NetworkReadingStream reader(connection);
    NetworkWritingStream writer(connection);

    while (true) {
        Packet unit;

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

            connection.Disconnect();

            std::cout << client_info.second << " disconnected!\n";

            delete reinterpret_cast<Connection *>(_client);

            return 0;
        }

        for (const auto &i : clients) {
            if (i.first != _client) {
                writer.Reset(*i.first);
                writer.Write(unit);
            }
        }

        writer.Reset(connection);
    }

    return 0;
}

int main() {
    using namespace cloud_storage;

    InitializeCriticalSection(&client_section);

    std::string port;

    std::cout << "Port: ";
    std::cin >> port;

    network::Listener listener(port);

    char addr[100];

    while (true) {
        network::Connection *connection = new network::Connection(listener.Accept());

        getnameinfo((const sockaddr *)&connection->GetSocketInfo().address,
            connection->GetSocketInfo().address_length, addr, sizeof(addr),
            NULL, 0, NI_NUMERICHOST);

        std::cout << addr << " connected!" << std::endl;

        EnterCriticalSection(&client_section);

        clients.emplace_back(connection, addr);

        LeaveCriticalSection(&client_section);

        QueueUserWorkItem(ClientHandler, connection, 0);
    }

    DeleteCriticalSection(&client_section);

    return 0;
}
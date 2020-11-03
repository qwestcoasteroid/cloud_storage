#include <windows.h>

#include <iostream>

#include "server.hpp"
#include "client.hpp"
#include "profile.hpp"
#include "network_writing_stream.hpp"
#include "network_reading_stream.hpp"

DWORD WINAPI ClientHandler(void *__client) {
    using cloud_storage::network::Client;
    using cloud_storage::network::TransmissionUnit;
    using cloud_storage::network::NetworkReadingStream;
    using cloud_storage::network::NetworkWritingStream;
    using cloud_storage::network::UnitType;
    using cloud_storage::network::DataType;

    Client &client = *reinterpret_cast<Client *>(__client);

    NetworkReadingStream reader(client);
    NetworkWritingStream writer(client);

    do {
        TransmissionUnit unit;

        try {
            unit = reader.Read();
        }
        catch (...) {
            std::cout << "Client " << __client << " disconnected!\n";

            client.Disconnect();

            free(__client);

            return 0;
        }
        
        cloud_storage::stored_data::Profile profile;

        profile.username = "sakuyamaxanadu";
        profile.max_storage = (1 << 10) * 1024;
        profile.used_storage = (1 << 10) * 512;

        auto [ptr, size] = profile.Serialize();

        unit = cloud_storage::network::MakeRespond(DataType::kProfile,
            ptr, size);

        writer.Write(unit);

        std::cout << "Message sent! from " << GetCurrentThreadId() << " thread." << std::endl;

    } while (true);

    free(__client);

    return 0;
}

int main() {
    cloud_storage::network::Server server("43000");

    do {
        cloud_storage::network::Client *client =
            (cloud_storage::network::Client *)malloc(
            sizeof(cloud_storage::network::Client));

        *client = server.Accept();

        char ipaddr[100];

        getnameinfo(reinterpret_cast<const sockaddr *>(&client->GetSocketInfo().address),
            client->GetSocketInfo().address_length, ipaddr,
            sizeof(ipaddr), nullptr, 0, NI_NUMERICHOST);

        std::cout << "Client [" << ipaddr << "] connected!\n\n";

        QueueUserWorkItem(ClientHandler, client, WT_EXECUTEDEFAULT);

    } while (true);

    return 0;
}
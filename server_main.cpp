#include <windows.h>

#include <iostream>

#include "server.hpp"
#include "client.hpp"
#include "file.hpp"
#include "network_writing_stream.hpp"
#include "network_reading_stream.hpp"

#define FRAGMENT_SIZE (64 * (1 << 10))

DWORD WINAPI ClientHandler(void *_client) {
    using cloud_storage::network::Client;
    using cloud_storage::network::TransmissionUnit;
    using cloud_storage::network::NetworkReadingStream;
    using cloud_storage::network::NetworkWritingStream;
    using cloud_storage::network::DataType;

    Client &client = *reinterpret_cast<Client *>(_client);

    NetworkReadingStream reader(client);
    NetworkWritingStream writer(client);

    do {
        TransmissionUnit unit;

        try {
            unit = reader.Read();
        }
        catch (...) {
            std::cout << "Client " << _client << " disconnected!\n";

            client.Disconnect();

            free(_client);

            return 0;
        }

        if (unit.GetHeader().data_type == DataType::kFile && !unit.GetHeader().respond) {
            std::string file_name(unit.GetData().get());

            HANDLE requested_file = CreateFile(file_name.c_str(),
                GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            DWORD file_size;
            GetFileSize(requested_file, &file_size);

            HANDLE file_map = CreateFileMapping(requested_file, NULL,
                PAGE_READONLY, 0, 0, NULL);

            LPVOID file_ptr = MapViewOfFile(file_map, FILE_MAP_READ, 0, 0, 0);

            cloud_storage::stored_data::File file;

            file.size = file_size;
            file.name = file_name;
            GetSystemTime(&file.creation_time);
            file.editing_time = file.creation_time;

            auto [ptr, size] = file.Serialize();

            TransmissionUnit unit = cloud_storage::network::MakeRespond(DataType::kFile, ptr, size);

            writer.Write(unit);

            

            UnmapViewOfFile(file_ptr);
            CloseHandle(file_map);
            CloseHandle(requested_file);
        }

        std::cout << "Message sent! from " << GetCurrentThreadId() << " thread." << std::endl;

    } while (true);

    free(_client);

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
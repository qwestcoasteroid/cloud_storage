#include <windows.h>

#include <iostream>

#include "listener.hpp"
#include "connection.hpp"
#include "file_info.hpp"
#include "network_writing_stream.hpp"
#include "network_reading_stream.hpp"

#define FRAGMENT_SIZE (64 * (1 << 10))

DWORD WINAPI ClientHandler(void *_client) {
    using cloud_storage::network::Connection;
    using cloud_storage::network::Packet;
    using cloud_storage::network::NetworkReadingStream;
    using cloud_storage::network::NetworkWritingStream;
    using cloud_storage::network::DataType;

    Connection &connection = *reinterpret_cast<Connection *>(_client);

    NetworkReadingStream reader(connection);
    NetworkWritingStream writer(connection);

    do {
        Packet unit;

        try {
            unit = reader.Read();
        }
        catch (...) {
            std::cout << "Connection " << _client << " disconnected!\n";

            connection.Disconnect();

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

            cloud_storage::stored_data::FileInfo file;

            file.size = file_size;
            file.name = file_name;
            GetSystemTime(&file.creation_time);
            file.editing_time = file.creation_time;

            auto [ptr, size] = file.Serialize();

            Packet unit = cloud_storage::network::MakeRespond(DataType::kFile, ptr, size);

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
    cloud_storage::network::Listener listener("43000");

    do {
        cloud_storage::network::Connection *connection =
            (cloud_storage::network::Connection *)malloc(
            sizeof(cloud_storage::network::Connection));

        *connection = listener.Accept();

        char ipaddr[100];

        getnameinfo(reinterpret_cast<const sockaddr *>(&connection->GetSocketInfo().address),
            connection->GetSocketInfo().address_length, ipaddr,
            sizeof(ipaddr), nullptr, 0, NI_NUMERICHOST);

        std::cout << "Connection [" << ipaddr << "] connected!\n\n";

        QueueUserWorkItem(ClientHandler, connection, WT_EXECUTEDEFAULT);

    } while (true);

    return 0;
}
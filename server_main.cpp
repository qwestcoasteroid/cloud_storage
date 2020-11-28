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

    std::unique_ptr<Connection> connection(
        reinterpret_cast<Connection *>(_client)
    );

    NetworkReadingStream reader(*connection);
    NetworkWritingStream writer(*connection);

    do {
        Packet unit;

        try {
            unit = reader.Read();
        }
        catch (...) {
            std::cout << "Client " << _client << " disconnected!\n";

            connection->Disconnect();

            return 0;
        }

        if (unit.header.data_type == DataType::kFileInfo) {
            cloud_storage::network::NetworkBuffer net_buf;

            std::cout << "File requested!\n";

            net_buf.buffer = unit.data;
            net_buf.data_type = unit.header.data_type;
            net_buf.length = unit.header.data_length;

            std::cout << "Allocating disk space!" << std::endl;

            cloud_storage::stored_data::FileInfo file_req(net_buf);

            std::cout << "File name: " << file_req.name << std::endl;

            std::string dir("F:\\client_dir\\");

            HANDLE requested_file = CreateFile((dir + file_req.name).c_str(),
                GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL, NULL
            );

            std::cout << "File created!" << std::endl;

            Packet answer;

            answer.header.data_length = 0;
            answer.header.data_type =
                cloud_storage::network::DataType::kFileInfo;

            if (requested_file == INVALID_HANDLE_VALUE) {
                answer.header.error_code =
                    cloud_storage::network::ErrorCode::kError;

                std::cout << "Error opening file! " <<
                    GetLastError() << std::endl;

                writer.Write(answer);

                continue;
            }

            std::cout << "Sending answer!" << std::endl;

            answer.header.error_code =
                cloud_storage::network::ErrorCode::kSuccess;

            answer.data = std::make_shared<char[]>(0);

            writer.Write(answer);

            std::cout << "Mapping file!" << std::endl;

            LARGE_INTEGER file_size{};
            file_size.QuadPart = file_req.size;

            SetFilePointerEx(requested_file, file_size, NULL, FILE_BEGIN);

            SetEndOfFile(requested_file);

            HANDLE file_map = CreateFileMapping(requested_file, NULL,
                PAGE_READWRITE, 0, 0, NULL);

            DWORD err = GetLastError();

            LPVOID file_ptr = MapViewOfFile(file_map, FILE_MAP_WRITE, 0, 0, 0);

            LPSTR tmp_file_ptr = reinterpret_cast<LPSTR>(file_ptr);

            size_t recvd = 0;

            bool fail = false;

            std::cout << "Receiving file: " << file_req.name << std::endl;

            std::streamsize field_size = std::to_string(file_req.size).size();

            do {
                try {
                    unit = reader.Read();
                }
                catch (...) {
                    fail = true;
                    break;
                }
                

                if (unit.header.error_code !=
                    cloud_storage::network::ErrorCode::kSuccess) {

                }

                std::memcpy(tmp_file_ptr, unit.data.get(),
                    unit.header.data_length
                );

                tmp_file_ptr += unit.header.data_length;

                recvd += unit.header.data_length;
                
                std::cout << "Received: ";
                std::cout.width(field_size);
                std::cout << recvd;
                std::cout <<  " of ";
                std::cout.width(field_size);
                std::cout << file_req.size << '\r';
                
            } while (recvd != file_req.size);

            std::cout << std::endl;

            connection->Disconnect();

            // FlushViewOfFile(file_ptr, 0);
            UnmapViewOfFile(file_ptr);
            CloseHandle(file_map);
            CloseHandle(requested_file);

            if (fail) {
                DeleteFileA((dir + file_req.name).c_str());
            } 
        }

        std::cout << "File received! in [" << GetCurrentThreadId()
            << "] thread." << std::endl;

    } while (true);

    return 0;
}

int main() {
    cloud_storage::network::Listener listener("43000");

    do {
        auto connection = listener.Accept();

        char ipaddr[100];

        getnameinfo(reinterpret_cast<const sockaddr *>(
            &connection->GetSocketInfo().address),
            connection->GetSocketInfo().address_length, ipaddr,
            sizeof(ipaddr), nullptr, 0, NI_NUMERICHOST
        );

        std::cout << "Client [" << ipaddr << "] connected!\n\n";

        QueueUserWorkItem(ClientHandler, connection.release(),
            WT_EXECUTELONGFUNCTION);

    } while (true);

    return 0;
}
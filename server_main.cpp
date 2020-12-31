#include <Windows.h>

#include <iostream>
#include <cstring>

#include "protocol.hpp"
#include "listener.hpp"
#include "connection.hpp"
#include "file_info.hpp"
#include "file_upload.hpp"
#include "file_download.hpp"
#include "file_part.hpp"
#include "network_stream.hpp"
#include "exceptions.hpp"
#include "file_operations.hpp"

cloud_storage::messages::FileUploadResponse CreateFileUploadResponse(
    const cloud_storage::messages::FileUploadRequest &_request
);

cloud_storage::messages::FileDownloadResponse CreateFileDownloadResponse(
    const cloud_storage::messages::FileDownloadRequest &_request
);

DWORD WINAPI ClientHandler(void *_client) {
    using namespace cloud_storage::network;
    using namespace cloud_storage::messages;

    std::string dir("F:\\cloud_storage_test\\cloud_directory\\");

    std::unique_ptr<Connection> connection(
        reinterpret_cast<Connection *>(_client)
    );

    NetworkStream stream(*connection);

    do {
        Packet unit;

        try {
            unit = stream.Read();
        }
        catch (exceptions::ConnectionClosed &e) {
            std::cout << "Client [" << _client << "] disconnected!\n";
            connection->Disconnect();
            return 0;
        }
        catch (exceptions::SocketError &e) {
            std::cout << "Error [" << e.GetErrorCode() << "] ocurred!\n";
            return 0;
        }

        switch (unit.header.message_id) {
        case MessageID::kFileUploadRequest: {
            FileUploadRequest request(
                CreateNetworkBuffer(unit)
            );
            auto response = CreateFileUploadResponse(request);
            AppendPacket(unit, response.Serialize());
            try {
                stream.Write(unit);
                std::cout << "Receiving file from [" << _client << "]...\n";
                ReceiveFile(dir + request.name, request.size,
                    response.chunk_size, stream
                );
            }
            catch (exceptions::ConnectionClosed &e) {
                std::cout << "Client [" << _client << "] disconnected!\n";
                connection->Disconnect();
                return 0;
            }
            catch (exceptions::SocketError &e) {
                std::cout << "Error [" << e.GetErrorCode() << "] ocurred!\n";
                return 0;
            } 
            break;
        }
        case MessageID::kFileDownloadRequest: {
            FileDownloadRequest request(
                CreateNetworkBuffer(unit)
            );
            auto response = CreateFileDownloadResponse(request);
            AppendPacket(unit, response.Serialize());
            try {
                stream.Write(unit);
                std::cout << "Sending file to [" << _client << "]...\n";
                SendFile(dir + response.name, response.size,
                    response.chunk_size, stream
                );
            }
            catch (exceptions::SocketError &e) {
                std::cout << "Error [" << e.GetErrorCode() << "] ocurred!\n";
                return 0;
            } 
            break;
        }
        default:
            std::cout << "Unknown request from client ["
                << _client << "]!\n";
            connection->Disconnect();
            return 0;
        }

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

        std::cout << "Client [" << ipaddr << "] connected!\n";

        QueueUserWorkItem(ClientHandler, connection.release(),
            WT_EXECUTELONGFUNCTION
        );

    } while (true);

    return 0;
}

cloud_storage::messages::FileUploadResponse CreateFileUploadResponse(
    const cloud_storage::messages::FileUploadRequest &_request) {

    cloud_storage::messages::FileUploadResponse result;

    result.name = _request.name;

    ULARGE_INTEGER free_space{};
    
    GetDiskFreeSpaceExA("F:\\", &free_space, NULL, NULL);

    if (free_space.QuadPart < _request.size) {
        result.difference = _request.size - free_space.QuadPart;
    }

    SYSTEM_INFO sys_info{};

    GetSystemInfo(&sys_info);

    result.chunk_size = sys_info.dwAllocationGranularity;

    return result;
}

cloud_storage::messages::FileDownloadResponse CreateFileDownloadResponse(
    const cloud_storage::messages::FileDownloadRequest &_request) {

    cloud_storage::messages::FileDownloadResponse result;

    std::string full_path("F:\\cloud_storage_test\\cloud_directory\\");

    full_path.append(_request.name);

    HANDLE file = CreateFileA(full_path.c_str(), GENERIC_READ,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        return result;
    }

    result.name = _request.name;

    LARGE_INTEGER file_size{};

    GetFileSizeEx(file, &file_size);

    result.size = file_size.QuadPart;

    SYSTEM_INFO sys_info{};

    GetSystemInfo(&sys_info);

    result.chunk_size = sys_info.dwAllocationGranularity;

    CloseHandle(file);

    return result;
}
#include <iostream>
#include <cstring>

#include "connection.hpp"
#include "file_info.hpp"
#include "network_stream.hpp"
#include "help_tools.hpp"
#include "file_upload.hpp"
#include "file_download.hpp"
#include "file_part.hpp"
#include "exceptions.hpp"
#include "file_operations.hpp"

cloud_storage::messages::FileUploadRequest CreateUploadFileRequest(
    std::string_view _name
);
cloud_storage::messages::FileDownloadRequest CreateDownloadFileRequest(
    std::string_view _name
);

int main() {
    using namespace cloud_storage::network;
    using namespace cloud_storage::messages;
    
    std::string file_name, command;
    std::string dir_name("F:\\cloud_storage_test\\local_directory\\");

    std::cin >> command >> std::ws;
    std::getline(std::cin, file_name);

    Connection connection("5.3.205.219", "43000");

    connection.Connect();

    NetworkStream stream(connection);

    Packet packet;

    if (command == "upload") {
        try {
            FileUploadRequest upload_request =
                CreateUploadFileRequest(file_name);
            AppendPacket(packet, upload_request.Serialize());
            stream.Write(packet);
            packet = stream.Read();
            FileUploadResponse upload_response(CreateNetworkBuffer(packet));
            if (upload_response.difference != 0) {
                std::cerr << "Not enough space! Need ["
                    << upload_response.difference
                    << "] more bytes!" << std::endl;
                connection.Disconnect();
                return 0;
            }
            std::cout << "Sending file to [" << &connection << "]...\n";
            SendFile(dir_name + file_name, upload_request.size,
                upload_response.chunk_size, stream
            );
        }
        catch (exceptions::ConnectionClosed &e) {
            std::cout << "Remote peer disconnected!\n";
            connection.Disconnect();
            return 0;
        }
        catch (exceptions::SocketError &e) {
            std::cout << "Error [" << e.GetErrorCode() << "] ocurred!\n";
            return 0;
        }
    }
    else if (command == "download") {
        try {
            FileDownloadRequest download_request =
            CreateDownloadFileRequest(file_name);
                AppendPacket(packet, download_request.Serialize());
            stream.Write(packet);
            packet = stream.Read();
            FileDownloadResponse download_response(
                CreateNetworkBuffer(packet)
            );
            if (download_response.name.empty()) {
                std::cerr << "No such file!" << std::endl;
                connection.Disconnect();
                return 0;
            }
            std::cout << "Receiving file from [" << &connection << "]...\n";
            ReceiveFile(dir_name + file_name, download_response.size,
                download_response.chunk_size, stream
            );
        }
        catch (exceptions::ConnectionClosed &e) {
            std::cout << "Remote peer disconnected!\n";
            connection.Disconnect();
            return 0;
        }
        catch (exceptions::SocketError &e) {
            std::cout << "Error [" << e.GetErrorCode() << "] ocurred!\n";
            return 0;
        }
    }
    else {
        std::cout << "Unknown command!" << std::endl;
        connection.Disconnect();
        return 0;
    }

    connection.Disconnect();
    
    return 0;
}

cloud_storage::messages::FileUploadRequest CreateUploadFileRequest(
    std::string_view _name) {

    cloud_storage::messages::FileUploadRequest result;

    std::string full_path("F:\\cloud_storage_test\\local_directory\\");

    full_path.append(_name);

    HANDLE file = CreateFileA(full_path.c_str(), GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        throw std::invalid_argument("No such file!");
    }

    LARGE_INTEGER file_size{};

    GetFileSizeEx(file, &file_size);

    result.name = _name;
    result.size = file_size.QuadPart;

    CloseHandle(file);

    return result;
}

cloud_storage::messages::FileDownloadRequest CreateDownloadFileRequest(
    std::string_view _name) {
        
    cloud_storage::messages::FileDownloadRequest result;

    result.name = _name;

    return result;
}
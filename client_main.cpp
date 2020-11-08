#include <iostream>

#include "client.hpp"
#include "file.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "help_tools.hpp"

int main() {
    std::string file_name;

    std::cout << "Enter file name: ";
    std::cin >> file_name;
    
    cloud_storage::network::Client client("127.0.0.1", "43000");

    client.Connect();

    cloud_storage::network::NetworkReadingStream reader(client);
    cloud_storage::network::NetworkWritingStream writer(client);

    size_t packets_received = 0;

    do {
        auto unit = cloud_storage::network::MakeRequest(
            cloud_storage::network::DataType::kFile, file_name);

        writer.Write(unit);

        unit = reader.Read();

        if (unit.GetHeader().error) {
            std::cerr << "Bad request!" << std::endl;
            continue;
        }

        cloud_storage::stored_data::File file(unit);

        if (unit.GetHeader().data_type == cloud_storage::network::DataType::kFile) {
            std::cout << "File received!" << std::endl;
            ++packets_received;
        }

        std::cout << "File name: " << file.name << std::endl;
        std::cout << "File size: " << file.size  << " bytes" << std::endl;
        std::cout << "Creation year: " << file.creation_time.wYear  << std::endl;
        std::cout << "Editing year: " << file.editing_time.wYear  << std::endl << std::endl;

        Sleep(500);

    } while (packets_received != 12);

    client.Disconnect();

    try {
        reader.Read();
    }
    catch (...) {
        std::cout << "Disconnected from " << &client << std::endl;
    }

    return 0;
}
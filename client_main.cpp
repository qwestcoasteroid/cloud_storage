#include <iostream>

#include "client.hpp"
#include "profile.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "help_tools.hpp"

int main() {
    cloud_storage::network::Client client("127.0.0.1", "43000");

    client.Connect();

    std::cout << "Can't connect!\n\n";

    std::cout << "Connected!\n\n";

    cloud_storage::network::NetworkReadingStream reader(client);
    cloud_storage::network::NetworkWritingStream writer(client);

    size_t packets_received = 0;

    do {
        auto unit = cloud_storage::network::MakeRequest(
            cloud_storage::network::DataType::kProfile, "sakuyamaxanadu");

        writer.Write(unit);

        unit = reader.Read();

        if (unit.GetHeader().unit_type == cloud_storage::network::UnitType::kError) {
            std::cerr << "Bad request!" << std::endl;
            continue;
        }

        cloud_storage::stored_data::Profile profile(unit);

        if (unit.GetHeader().data_type == cloud_storage::network::DataType::kProfile) {
            std::cout << "Profile received!" << std::endl;
            ++packets_received;
        }

        std::cout << "Username: " << profile.username << std::endl;
        std::cout << "Max storage: " << profile.max_storage  << " bytes" << std::endl;
        std::cout << "Used storage: " << profile.used_storage  << " bytes" << std::endl;

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
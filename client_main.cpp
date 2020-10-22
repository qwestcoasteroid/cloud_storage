#include <iostream>

#include "client.hpp"
#include "profile.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "help_tools.hpp"

int main() {
    cloud_storage::network::Client client("127.0.0.1", "43000");

    if (!client.Connect()) {
        std::cout << "Can't connect!\n\n";
    }
    else {
        std::cout << "Connected!\n\n";

        cloud_storage::network::NetworkReadingStream reader(client);
        cloud_storage::network::NetworkWritingStream writer(client);

        do {
            cloud_storage::network::TransmissionUnit unit;

            unit.header.data_type = cloud_storage::network::DataType::kProfile;
            unit.header.unit_type = cloud_storage::network::UnitType::kRequest;

            writer.Write(unit);

            unit = reader.Read();

            if (unit.header.unit_type == cloud_storage::network::UnitType::kError) {
                std::cerr << "Bad request!" << std::endl;
                continue;
            }

            cloud_storage::stored_data::Profile profile(unit);

            if (unit.header.data_type == cloud_storage::network::DataType::kProfile) {
                std::cout << "Profile received!" << std::endl;
            }

            std::cout << "Username: " << profile.username << std::endl;
            std::cout << "Max storage: " << profile.max_storage  << " bytes" << std::endl;
            std::cout << "Used storage: " << profile.used_storage  << " bytes" << std::endl;

            Sleep(500);

        } while (true);
    }

    return 0;
}
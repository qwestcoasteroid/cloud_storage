#include <iostream>

#include "client.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "profile.hpp"

int main() {
    using namespace cloud_storage;

    network::Client connection("127.0.0.1", "43000");

    connection.Connect();

    auto request = network::MakeRequest(network::DataType::kProfile,
        "sakuyamaxanadu");

    network::NetworkWritingStream writer(connection);

    writer.Write(request);

    network::NetworkReadingStream reader(connection);

    auto respond = reader.Read();

    if (respond.GetHeader().unit_type == network::UnitType::kError) {
        std::cerr << respond.GetData().get() << std::endl;
    }
    else {
        stored_data::Profile profile(respond);
    }

    return 0;
}
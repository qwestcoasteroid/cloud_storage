#include <iostream>

#include "client.hpp"
#include "profile.hpp"

int main() {
    cloud_storage::network::Client client("127.0.0.1", "43000");

    if (!client.Connect()) {
        std::cout << "Can't connect!\n\n";
    }
    else {
        std::cout << "Connected!\n\n";

        char buffer[100];

        recv(client.GetConnectionInfo().socket, buffer, sizeof(buffer), 0);

        cloud_storage::stored_data::Profile prof;

        prof.Deserialize(buffer);

        std::cout << "Username: " << prof.username << std::endl;
        std::cout << "Max storage: " << prof.max_storage  << " bytes" << std::endl;
        std::cout << "Used storage: " << prof.used_storage  << " bytes" << std::endl;
    }

    return 0;
}
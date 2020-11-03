#include <windows.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "client.hpp"
#include "server.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "message.hpp"

DWORD WINAPI HandleMessages(void *_arg) {
    using namespace cloud_storage;

    network::Client &client = *reinterpret_cast<network::Client *>(_arg);

    network::NetworkReadingStream reader(client);

    network::TransmissionUnit unit;

    while (true) {
        try {
            unit = reader.Read();
        }
        catch (...) {
            client.Disconnect();
            std::cout << "Disconnected from server!\n";
            exit(0);
        }

        stored_data::Message msg(unit);

        std::cout << msg.sender << ": " << msg.msg << std::endl;
    }

    return 0;
}

int main() {
    using namespace cloud_storage;

    std::string port, ip, msg, nickname;

    std::cout << "Remote IP: ";
    std::cin >> ip;
    std::cout << "Remote port: ";
    std::cin >> port;
    std::cout << "Your nickname: ";
    std::cin >> nickname;

    network::Client client(ip, port);

    client.Connect();

    QueueUserWorkItem(HandleMessages, &client, 0);

    network::NetworkWritingStream writer(client);

    stored_data::Message message;

    message.sender = nickname;

    network::TransmissionUnit unit;

    while (true) {
        std::cin >> std::ws;    

        std::getline(std::cin, message.msg);

        if (message.msg == "//quit") {
            break;
        }

        auto [ptr, size] = message.Serialize();

        unit = network::MakeRespond(network::DataType::kMessage, ptr, size);

        writer.Write(unit);
    }

    client.Disconnect();

    try {
        network::NetworkReadingStream reader(client);

        reader.Read();
    }
    catch (...) {
        std::cout << "Disconnected from server!\n";
    }

    return 0;
}
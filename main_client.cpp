#include <windows.h>

#include <iostream>
#include <vector>
#include <algorithm>

#include "connection.hpp"
#include "listener.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "message.hpp"

DWORD WINAPI HandleMessages(void *_arg) {
    using namespace cloud_storage;

    network::Connection &connection = *reinterpret_cast<network::Connection *>(_arg);

    network::NetworkReadingStream reader(connection);

    network::Packet unit;

    while (true) {
        try {
            unit = reader.Read();
        }
        catch (...) {
            connection.Disconnect();
            std::cout << "Disconnected from listener!\n";
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

    network::Connection connection(ip, port);

    connection.Connect();

    QueueUserWorkItem(HandleMessages, &connection, 0);

    network::NetworkWritingStream writer(connection);

    stored_data::Message message;

    message.sender = nickname;

    network::Packet unit;

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

    connection.Disconnect();

    try {
        network::NetworkReadingStream reader(connection);

        reader.Read();
    }
    catch (...) {
        std::cout << "Disconnected from listener!\n";
    }

    return 0;
}
#include "network_reading_stream.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "help_tools.hpp"

namespace cloud_storage::network {
    static void ReceiveDataStream(SOCKET socket, char *buffer, size_t size) {
        if (size == 0) {
            return;
        }

        int received{ 0 };

        do {
            int result = recv(socket, buffer, size, 0);

            if (result == 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Remote node disconnected!\n";
#endif // DEBUG_OUTPUT
                // throw
            }
            else if (result < 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling recv() ("
                    << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
                // throw
            }

            received += result;

        } while (static_cast<size_t>(received) != size);
    }

    void PerformStreamReading(const SocketInfo &socket_info,
        TransmissionUnit &unit) {

        ReceiveDataStream(socket_info.socket,
            reinterpret_cast<char *>(&unit.GetHeader()),
            sizeof(unit.GetHeader()));

        unit.GetHeader().data_length =
            service::ToHostRepresentation(unit.GetHeader().data_length);
        unit.GetHeader().data_type =
            service::ToHostRepresentation(unit.GetHeader().data_type);
        unit.GetHeader().unit_type =
            service::ToHostRepresentation(unit.GetHeader().unit_type);

        unit.SetData(nullptr, unit.GetHeader().data_length);

        ReceiveDataStream(socket_info.socket, unit.GetData().get(),
            unit.GetHeader().data_length);
    }

    TransmissionUnit NetworkReadingStream::Read() {
        TransmissionUnit result;

        switch (client_->GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamReading(client_->GetSocketInfo(), result);
            break;
        default:
#ifdef DEBUG_OUTPUT
                std::cerr << "Unknown protocol used while"
                    "performing reading operation!\n";
#endif // DEBUG_OUTPUT
            // throw
            break;
        }

        return result;
    }
} // namespace cloud_storage::network
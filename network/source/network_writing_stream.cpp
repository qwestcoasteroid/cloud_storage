#include "network_writing_stream.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "serializable.hpp"
#include "help_tools.hpp"

namespace cloud_storage::network {
    static void SendDataStream(SOCKET socket,
        const char *buffer, size_t size) {

        if (size == 0) {
            return;
        }

        int sent{ 0 };

        do {
            int result = send(socket, buffer, size, 0);

            if (result < 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling send() ("
                    << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
                // throw
            }

            sent += result;

        } while (static_cast<size_t>(sent) != size);
    }

    void PerformStreamWriting(const SocketInfo &socket_info,
        const TransmissionUnit &unit) {

        Header header = unit.GetHeader();

        header.data_length =
            service::ToNetworkRepresentation(header.data_length);
        header.data_type =
            service::ToNetworkRepresentation(header.data_type);
        header.unit_type =
            service::ToNetworkRepresentation(header.unit_type);
        
        SendDataStream(socket_info.socket,
            reinterpret_cast<const char *>(&header),
            sizeof(header));
        
        SendDataStream(socket_info.socket, unit.GetData().get(),
            unit.GetHeader().data_length);
    }

    void NetworkWritingStream::Write(const TransmissionUnit &unit) {
        switch (client_->GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamWriting(client_->GetSocketInfo(), unit);
            break;
        default:
#ifdef DEBUG_OUTPUT
            std::cerr << "Unknown protocol used while"
                "performing writing operation!\n";
#endif // DEBUG_OUTPUT
            // throw
            break;
        }
    }
} // namespace cloud_storage::network
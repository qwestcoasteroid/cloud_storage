#include "network_reading_stream.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "help_tools.hpp"

namespace cloud_storage::network {
    static void ReceiveDataStream(SOCKET _socket, char *_buffer,
        size_t _size) {
            
        if (_size == 0) {
            return;
        }

        int received{ 0 };

        do {
            int result = recv(_socket, _buffer, _size, 0);

            if (result == 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Remote node disconnected!\n";
#endif // DEBUG_OUTPUT
                throw std::runtime_error("error");
                // throw
            }
            else if (result < 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling recv() ("
                    << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
                throw std::runtime_error("error");
                // throw
            }

            received += result;

        } while (static_cast<size_t>(received) != _size);
    }

    void PerformStreamReading(const SocketInfo &_socket_info,
        Packet &_packet) {

        ReceiveDataStream(_socket_info.socket,
            reinterpret_cast<char *>(&_packet.GetHeader()),
            sizeof(_packet.GetHeader()));

        _packet.GetHeader().data_length =
            service::ToHostRepresentation(_packet.GetHeader().data_length);

        _packet.SetData(nullptr, _packet.GetHeader().data_length);

        ReceiveDataStream(_socket_info.socket, _packet.GetData().get(),
            _packet.GetHeader().data_length);
    }

    Packet NetworkReadingStream::Read() {
        Packet result;

        switch (connection_->GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamReading(connection_->GetSocketInfo(), result);
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
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

        do {
            int result = recv(_socket, _buffer, _size, 0);

            DWORD err = WSAGetLastError();

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

            _buffer += result;
            _size -= result;

        } while (_size != 0);
    }

    void PerformStreamReading(const SocketInfo &_socket_info,
        Packet &_packet) {

        ReceiveDataStream(_socket_info.socket,
            reinterpret_cast<char *>(&_packet.header),
            sizeof(_packet.header));

        PackNetworkHeader(_packet.header, false);

        _packet.data = std::shared_ptr<char[]>(new char[_packet.header.data_length]);

        ReceiveDataStream(_socket_info.socket, _packet.data.get(),
            _packet.header.data_length);
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
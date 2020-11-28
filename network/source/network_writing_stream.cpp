#include "network_writing_stream.hpp"

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "serializable.hpp"
#include "help_tools.hpp"

namespace cloud_storage::network {
    static void SendDataStream(SOCKET _socket,
        const char *_buffer, size_t _size) {

        if (_size == 0) {
            return;
        }

        do {
            int result = send(_socket, _buffer, _size, 0);

            DWORD err = WSAGetLastError();

            if (result < 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling send() ("
                    << WSAGetLastError() << ")" << std::endl;
#endif // DEBUG_OUTPUT
                throw std::runtime_error("error");
                // throw
            }

            _buffer += result;
            _size -= result;

        } while (_size != 0);
    }

    void PerformStreamWriting(const SocketInfo &_socket_info,
        const Packet &_packet) {

        NetworkHeader header = _packet.header;
        
        PackNetworkHeader(header, true);
        
        SendDataStream(_socket_info.socket,
            reinterpret_cast<const char *>(&header), sizeof(header));
        
        SendDataStream(_socket_info.socket, _packet.data.get(),
            _packet.header.data_length);
    }

    void NetworkWritingStream::Write(const Packet &_packet) {
        switch (connection_->GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamWriting(connection_->GetSocketInfo(), _packet);
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
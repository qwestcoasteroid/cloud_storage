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

        int sent{ 0 };

        do {
            int result = send(_socket, _buffer, _size, 0);

            if (result < 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling send() ("
                    << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
                throw std::runtime_error("error");
                // throw
            }

            sent += result;

        } while (static_cast<size_t>(sent) != _size);
    }

    void PerformStreamWriting(const SocketInfo &_socket_info,
        const Packet &_packet) {

        Header header = _packet.GetHeader();
        
        header.data_length =
            service::ToNetworkRepresentation(header.data_length);
        
        SendDataStream(_socket_info.socket,
            reinterpret_cast<const char *>(&header), sizeof(header));
        
        SendDataStream(_socket_info.socket, _packet.GetData().get(),
            _packet.GetHeader().data_length);
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
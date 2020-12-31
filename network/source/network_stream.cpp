#include "network_stream.hpp"

#include <WinSock2.h>

#include "exceptions.hpp"

namespace cloud_storage::network {
    static void ReceiveDataStream(SOCKET _socket, char *_buffer,
        size_t _size) {
            
        if (_size == 0) {
            return;
        }

        do {
            int result = recv(_socket, _buffer, _size, 0);

            if (result == 0) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Remote node disconnected!\n";
#endif // DEBUG_OUTPUT
                throw exceptions::ConnectionClosed();
            }
            else if (result == SOCKET_ERROR) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling recv() ("
                    << WSAGetLastError() << ")\n";
#endif // DEBUG_OUTPUT
                throw exceptions::SocketError(WSAGetLastError());
            }

            _buffer += result;
            _size -= result;

        } while (_size != 0);
    }

    void PerformStreamReading(const SocketInfo &_socket_info,
        Packet &_packet) {

        ReceiveDataStream(_socket_info.socket,
            reinterpret_cast<char *>(&_packet.header),
            sizeof(_packet.header)
        );

        SwitchByteOrder(_packet.header);

        _packet.data = std::shared_ptr<char[]>(
            new char[_packet.header.data_length]
        );

        ReceiveDataStream(_socket_info.socket, _packet.data.get(),
            _packet.header.data_length
        );
    }

    Packet NetworkStream::Read() {
        Packet result;

        switch (connection_.GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamReading(connection_.GetSocketInfo(), result);
            break;
        default:
#ifdef DEBUG_OUTPUT
            std::cerr << "Unknown protocol used while"
                "performing reading operation!\n";
#endif // DEBUG_OUTPUT
            throw exceptions::UnknownProtocol();
            break;
        }

        return result;
    }

    static void SendDataStream(SOCKET _socket,
        const char *_buffer, size_t _size) {

        if (_size == 0) {
            return;
        }

        do {
            int result = send(_socket, _buffer, _size, 0);

            if (result == SOCKET_ERROR) {
#ifdef DEBUG_OUTPUT
                std::cerr << "Error calling send() ("
                    << WSAGetLastError() << ")" << std::endl;
#endif // DEBUG_OUTPUT
                throw exceptions::SocketError(WSAGetLastError());
            }

            _buffer += result;
            _size -= result;

        } while (_size != 0);
    }

    void PerformStreamWriting(const SocketInfo &_socket_info,
        const Packet &_packet) {

        NetworkHeader header = _packet.header;
        
        SwitchByteOrder(header);
        
        SendDataStream(_socket_info.socket,
            reinterpret_cast<const char *>(&header), sizeof(header));
        
        SendDataStream(_socket_info.socket, _packet.data.get(),
            _packet.header.data_length);
    }

    void NetworkStream::Write(const Packet &_packet) {
        switch (connection_.GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamWriting(connection_.GetSocketInfo(), _packet);
            break;
        default:
#ifdef DEBUG_OUTPUT
            std::cerr << "Unknown protocol used while"
                "performing writing operation!\n";
#endif // DEBUG_OUTPUT
            throw exceptions::UnknownProtocol();
            break;
        }
    }
} // namespace cloud_storage::network
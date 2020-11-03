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
        TransmissionUnit &_unit) {

        ReceiveDataStream(_socket_info.socket,
            reinterpret_cast<char *>(&_unit.GetHeader()),
            sizeof(_unit.GetHeader()));

        _unit.GetHeader().data_length =
            service::ToHostRepresentation(_unit.GetHeader().data_length);
        _unit.GetHeader().data_type =
            service::ToHostRepresentation(_unit.GetHeader().data_type);
        _unit.GetHeader().unit_type =
            service::ToHostRepresentation(_unit.GetHeader().unit_type);

        _unit.SetData(nullptr, _unit.GetHeader().data_length);

        ReceiveDataStream(_socket_info.socket, _unit.GetData().get(),
            _unit.GetHeader().data_length);
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
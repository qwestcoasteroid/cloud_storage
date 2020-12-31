#include "protocol.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::network {
    void SwitchByteOrder(NetworkHeader &_header) noexcept {
        _header.access_token =
            service::ByteOrderSwitcher::Switch(_header.access_token);
        _header.data_length =
            service::ByteOrderSwitcher::Switch(_header.data_length);
        _header.user_id =
            service::ByteOrderSwitcher::Switch(_header.user_id);
    }

    void AppendPacket(Packet &_packet, const NetworkBuffer &_buffer) noexcept {
        _packet.header.message_id = _buffer.message_id;
        _packet.header.data_length = _buffer.length;
        _packet.data = _buffer.buffer;
    }

    NetworkBuffer CreateNetworkBuffer(const Packet &_packet) noexcept {
        NetworkBuffer result;

        result.buffer = _packet.data;
        result.length = _packet.header.data_length;
        result.message_id = _packet.header.message_id;

        return result;
    }
} // namespace cloud_storage::network
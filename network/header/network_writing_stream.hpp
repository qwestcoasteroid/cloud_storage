#ifndef CLOUD_STORAGE_NETWORK_NETWORK_WRITING_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_NETWORK_WRITING_STREAM_HPP_

#include "basic_network_stream.hpp"
#include "protocol.hpp"
#include "serializable.hpp"

namespace cloud_storage::network {
    class NetworkWritingStream : public virtual BasicNetworkStream {
    public:
        inline NetworkWritingStream(const Connection &_connection) noexcept :
            BasicNetworkStream(_connection) {} 

        void Write(const Packet &_packet);
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_NETWORK_WRITING_STREAM_HPP_
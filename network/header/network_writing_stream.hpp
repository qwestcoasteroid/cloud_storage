#ifndef CLOUD_STORAGE_NETWORK_NETWORK_WRITING_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_NETWORK_WRITING_STREAM_HPP_

#include "basic_network_stream.hpp"
#include "protocol.hpp"
#include "serializable.hpp"

namespace cloud_storage::network {
    class NetworkWritingStream : public BasicNetworkStream {
    public:
        inline NetworkWritingStream(const Client &client) noexcept :
            BasicNetworkStream(client) {} 

        void Write(const TransmissionUnit &unit);
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_NETWORK_WRITING_STREAM_HPP_
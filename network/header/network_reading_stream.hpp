#ifndef CLOUD_STORAGE_NETWORK_NETWORK_READING_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_NETWORK_READING_STREAM_HPP_

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "basic_network_stream.hpp"
#include "protocol.hpp"
#include "serializable.hpp"

namespace cloud_storage::network {
    class NetworkReadingStream : public BasicNetworkStream {
    public:
        inline NetworkReadingStream(const Client &_client) noexcept :
            BasicNetworkStream(_client) {}

        TransmissionUnit Read();
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_NETWORK_READING_STREAM_HPP_
#ifndef CLOUD_STORAGE_NETWORK_NETWORK_READING_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_NETWORK_READING_STREAM_HPP_

#ifdef DEBUG_OUTPUT
#include <iostream>
#endif // DEBUG_OUTPUT

#include "basic_network_stream.hpp"
#include "protocol.hpp"
#include "serializable.hpp"

namespace cloud_storage::network {
    extern void PerformStreamReading(const SocketInfo &, TransmissionUnit &);

    class NetworkReadingStream : public BasicNetworkStream {
    public:
        inline NetworkReadingStream(const Client &client) noexcept :
            BasicNetworkStream(client) {}

        template<typename TransmittableType>
        TransmittableType Read();
    };

    template<typename TransmittableType>
    TransmittableType NetworkReadingStream::Read() {
        static_assert(std::is_move_constructible_v<TransmittableType>);

        TransmittableType result;

        switch (client_->GetSocketInfo().socket_type) {
        case SOCK_STREAM:
            PerformStreamReading(client_->GetSocketInfo(),
                static_cast<TransmissionUnit &>(result));
            break;
        default:
#ifdef DEBUG_OUTPUT
                std::cerr << "Unknown protocol used while"
                    "performing reading operation!\n";
#endif // DEBUG_OUTPUT
            // throw
            break;
        }

        static_cast<utility::Serializable &>(result).Deserialize(
            static_cast<TransmissionUnit &>(result).data.get());

        return result;
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_NETWORK_READING_STREAM_HPP_
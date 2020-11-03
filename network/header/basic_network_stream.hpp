#ifndef CLOUD_STORAGE_NETWORK_BASIC_NETWORK_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_BASIC_NETWORK_STREAM_HPP_

#include "client.hpp"

namespace cloud_storage::network {
    class BasicNetworkStream {
    public:
        inline explicit BasicNetworkStream(const Client &_client) :
            client_(&_client)  {}

        virtual ~BasicNetworkStream() = 0;

        inline void Reset(const Client &_new_client) {
            client_ = &_new_client;
        }
        // Reset()
        inline const Client &ServedClient() const {
            return *client_;
        }

    protected:
        const Client *client_;
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_BASIC_NETWORK_STREAM_HPP_
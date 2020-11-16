#ifndef CLOUD_STORAGE_NETWORK_BASIC_NETWORK_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_BASIC_NETWORK_STREAM_HPP_

#include "connection.hpp"

namespace cloud_storage::network {
    class BasicNetworkStream {
    public:
        inline explicit BasicNetworkStream(const Connection &_connection) :
            connection_(&_connection)  {}

        virtual ~BasicNetworkStream() = 0;

        inline void Reset(const Connection &_new_connection);

        inline const Connection &ServedClient() const;

    protected:
        const Connection *connection_;
    };

    void BasicNetworkStream::Reset(const Connection &_new_connection) {
        connection_ = &_new_connection;
    }
    
    const Connection &BasicNetworkStream::ServedClient() const {
        return *connection_;
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_BASIC_NETWORK_STREAM_HPP_
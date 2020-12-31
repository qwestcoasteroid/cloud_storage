#ifndef CLOUD_STORAGE_NETWORK_NETWORK_STREAM_HPP_
#define CLOUD_STORAGE_NETWORK_NETWORK_STREAM_HPP_

#include "connection.hpp"
#include "protocol.hpp"

namespace cloud_storage::network {
    class NetworkStream {
    public:
        inline explicit NetworkStream(const Connection &_connection) :
            connection_(_connection)  {}

        virtual inline ~NetworkStream();

        Packet Read();
        void Write(const Packet &_packet);

        inline const Connection &GetConnection() const;

    protected:
        const Connection &connection_;
    };

    NetworkStream::~NetworkStream() {}
    
    const Connection &NetworkStream::GetConnection() const {
        return connection_;
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_NETWORK_STREAM_HPP_
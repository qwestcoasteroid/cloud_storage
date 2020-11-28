#ifndef CLOUD_STORAGE_SERVICE_SERIALIZABLE_HPP_
#define CLOUD_STORAGE_SERVICE_SERIALIZABLE_HPP_

#include "protocol.hpp"

namespace cloud_storage::service {
    class Serializable {
    public:
        virtual network::NetworkBuffer Serialize() const = 0;
        virtual Serializable &Deserialize(const network::NetworkBuffer &) = 0;
    };
} // namespace cloud_storage::service

#endif // CLOUD_STORAGE_SERVICE_SERIALIZABLE_HPP_
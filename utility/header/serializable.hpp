#ifndef CLOUD_STORAGE_UTILITY_SERIALIZABLE_HPP_
#define CLOUD_STORAGE_UTILITY_SERIALIZABLE_HPP_

#include "protocol.hpp"

namespace cloud_storage::utility {
    class Serializable {
    public:
        virtual network::TransmissionUnit Serialize() const = 0;
        virtual Serializable &Deserialize(const void *buffer = nullptr) = 0;
    };
} // namespace cloud_storage::utility

#endif // CLOUD_STORAGE_UTILITY_SERIALIZABLE_HPP_
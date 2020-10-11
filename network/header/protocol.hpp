#ifndef CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
#define CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_

#include <memory>

namespace cloud_storage::network {
    enum class DataType : unsigned int {
        kFile           = 0xFF000000, kQuery          = 0xFE000000,
        kAuthorization  = 0xFD000000, kRegistration   = 0xFC000000,
        kProfile        = 0xFB000000
    };

    struct Header {
        size_t data_length{};
        DataType data_type;
    };

    struct TransmissionUnit {
        Header header;
        std::unique_ptr<void> data;
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
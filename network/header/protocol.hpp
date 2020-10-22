#ifndef CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
#define CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_

#include <memory>

namespace cloud_storage::network {
    enum class DataType : uint16_t {
        kFile           = 0xFF00, kQuery          = 0xFE00,
        kAuthorization  = 0xFD00, kRegistration   = 0xFC00,
        kProfile        = 0xFB00
    };

    enum class UnitType : uint16_t {
        kRequest    = 0x80,
        kRespond    = 0x40,
        kError      = 0x20
    };

    struct Header {
        uint32_t data_length{};
        DataType data_type;
        UnitType unit_type;
    };

    struct TransmissionUnit {
        inline TransmissionUnit() {}
        inline virtual ~TransmissionUnit() {}

        TransmissionUnit(TransmissionUnit &&unit) noexcept;
        TransmissionUnit &operator=(TransmissionUnit &&unit) noexcept;

        mutable Header header;
        std::unique_ptr<char> data;
    };
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
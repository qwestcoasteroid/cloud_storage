#ifndef CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
#define CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_

#include <memory>
#include <string_view>

// TODO: MakeError - send error code instead of message

namespace cloud_storage::network {
    enum class DataType : uint16_t {
        kFile           = 0xFF00, kQuery          = 0xFE00,
        kAuthorization  = 0xFD00, kRegistration   = 0xFC00,
        kProfile        = 0xFB00, kMessage        = 0xFA00
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

    class TransmissionUnit {
    public:
        inline TransmissionUnit() noexcept {}
        inline virtual ~TransmissionUnit() {}

        TransmissionUnit(TransmissionUnit &&_unit) noexcept;
        TransmissionUnit &operator=(TransmissionUnit &&_unit) noexcept;

        inline const Header &GetHeader() const;
        inline Header &GetHeader();
        inline const std::shared_ptr<char[]> &GetData() const;
        inline std::shared_ptr<char[]> &GetData();

        inline void SetHeader(Header _header);
        void SetData(const std::shared_ptr<char[]> &_buffer, size_t _size);

    private:
        Header header_;
        std::shared_ptr<char[]> data_;
    };

    TransmissionUnit MakeRequest(DataType _type, std::string_view _resource);
    TransmissionUnit MakeRespond(DataType _type,
        const std::shared_ptr<char[]> &_buffer, size_t _size);
    TransmissionUnit MakeError(DataType _type, std::string_view _message);

    const Header &TransmissionUnit::GetHeader() const {
        return header_;
    }

    Header &TransmissionUnit::GetHeader() {
        return header_;
    }

    const std::shared_ptr<char[]> &TransmissionUnit::GetData() const {
        return data_;
    }

    std::shared_ptr<char[]> &TransmissionUnit::GetData() {
        return data_;
    }

    void TransmissionUnit::SetHeader(Header _header) {
        header_ = _header;
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
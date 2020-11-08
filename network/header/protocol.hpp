#ifndef CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
#define CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_

#include <memory>
#include <string_view>

// TODO: MakeError - send error code instead of message

namespace cloud_storage::network {
    enum class DataType : uint8_t {
        kFile           = 0xFF, kQuery          = 0xFE,
        kAuthorization  = 0xFD, kRegistration   = 0xFC,
        kProfile        = 0xFB, kMessage        = 0xFA
    };

    enum class ErrorCode : uint8_t {
        kSuccess = 0x00
    };

    struct Header {
        uint32_t data_length{};
        DataType data_type;
        ErrorCode error_code;
        uint32_t fragment_size{};
        uint8_t respond : 1;
        uint8_t fragmentation : 1;
        uint8_t error : 1;
    private:
        uint8_t reserved : 5;
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
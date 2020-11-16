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
        ErrorCode error_code{ ErrorCode::kSuccess };
        uint8_t respond : 1;
        uint8_t fragment : 1;
        uint8_t error : 1;
    private:
        uint8_t reserved_ : 5;
    };

    class Packet {
    public:
        inline Packet() noexcept {}
        inline virtual ~Packet() {}

        Packet(Packet &&_packet) noexcept;
        Packet &operator=(Packet &&_packet) noexcept;

        inline const Header &GetHeader() const;
        inline Header &GetHeader();
        inline const std::shared_ptr<char[]> &GetData() const;
        inline std::shared_ptr<char[]> &GetData();

        inline void SetHeader(const Header &_header);
        void SetData(const std::shared_ptr<char[]> &_buffer, size_t _size);

    private:
        Header header_;
        std::shared_ptr<char[]> data_;
    };

    Packet MakeRequest(DataType _type, std::string_view _resource);
    Packet MakeRespond(DataType _type,
        const std::shared_ptr<char[]> &_buffer, size_t _size);
    Packet MakeError(DataType _type, std::string_view _message);

    const Header &Packet::GetHeader() const {
        return header_;
    }

    Header &Packet::GetHeader() {
        return header_;
    }

    const std::shared_ptr<char[]> &Packet::GetData() const {
        return data_;
    }

    std::shared_ptr<char[]> &Packet::GetData() {
        return data_;
    }

    void Packet::SetHeader(const Header &_header) {
        header_ = _header;
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_PROTOCOL_HPP_
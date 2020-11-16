#include "protocol.hpp"

#include <cstring>

namespace cloud_storage::network {
    Packet::Packet(Packet &&_packet) noexcept :
        header_(_packet.header_), data_(std::move(_packet.data_)) {}

    Packet &Packet::operator=
        (Packet &&_packet) noexcept {

        if (&_packet == this) {
            return *this;
        }

        header_ = _packet.header_;
        data_ = std::move(_packet.data_);
        
        return *this;
    }

    void Packet::SetData(const std::shared_ptr<char[]> &_buffer,
        size_t _size) {
            
        if (_size == 0) {
            data_.reset();
        }
        else if (_buffer == nullptr) {
            data_.reset(new char[_size]);
        }
        else {
            data_ = _buffer;
        }
        
        header_.data_length = _size;
    }

    Packet MakeRequest(DataType _type, std::string_view _resource) {
        Packet result;

        Header header;

        header.data_type = _type;
        header.respond = 0;
        header.error = 0;

        result.SetHeader(header);

        result.SetData(nullptr, _resource.size() + 1);

        std::memcpy(result.GetData().get(), _resource.data(),
            _resource.size());

        result.GetData().get()[_resource.size()] = '\0';

        return  result;
    }

    Packet MakeRespond(DataType _type,
        const std::shared_ptr<char[]> &_buffer, size_t _size) {

        Packet result;

        Header header;

        header.data_type = _type;
        header.respond = 1;
        header.error = 0;

        result.SetHeader(header);
        
        result.SetData(_buffer, _size);

        return result;
    }

    Packet MakeError(DataType _type, std::string_view _message) {
        Packet result = MakeRequest(_type, _message);

        result.GetHeader().error = 1;
        result.GetHeader().respond = 1;

        return result;
    }
} // namespace cloud_storage::network
#include "protocol.hpp"

#include <cstring>

namespace cloud_storage::network {
    TransmissionUnit::TransmissionUnit(TransmissionUnit &&_unit) noexcept :
        header_(_unit.header_), data_(std::move(_unit.data_)) {}

    TransmissionUnit &TransmissionUnit::operator=
        (TransmissionUnit &&_unit) noexcept {

        if (&_unit == this) {
            return *this;
        }

        header_ = _unit.header_;
        data_ = std::move(_unit.data_);
        
        return *this;
    }

    void TransmissionUnit::SetData(const std::shared_ptr<char[]> &_buffer,
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

    TransmissionUnit MakeRequest(DataType _type, std::string_view _resource) {
        TransmissionUnit result;

        Header header;

        header.data_type = _type;
        header.unit_type = UnitType::kRequest;

        result.SetHeader(header);

        result.SetData(nullptr, _resource.size() + 1);

        std::memcpy(result.GetData().get(), _resource.data(), _resource.size());

        result.GetData().get()[_resource.size()] = '\0';

        return  result;
    }

    TransmissionUnit MakeRespond(DataType _type,
        const std::shared_ptr<char[]> &_buffer, size_t _size) {

        TransmissionUnit result;

        Header header;

        header.data_type = _type;
        header.unit_type = UnitType::kRespond;

        result.SetHeader(header);
        
        result.SetData(_buffer, _size);

        return result;
    }

    TransmissionUnit MakeError(DataType _type, std::string_view _message) {
        TransmissionUnit result = MakeRequest(_type, _message);

        result.GetHeader().unit_type = UnitType::kError;

        return result;
    }
} // namespace cloud_storage::network
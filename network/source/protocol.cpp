#include "protocol.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::network {
    void PackNetworkHeader(NetworkHeader &_header, bool _to_network) noexcept {
        if (_to_network) {
            _header.access_token = service::ToNetworkRepresentation(
                _header.access_token
            );
            _header.data_length = service::ToNetworkRepresentation(
                _header.data_length
            );
            _header.user_id = service::ToNetworkRepresentation(
                _header.user_id
            );
        }
        else {
            _header.access_token = service::ToHostRepresentation(
                _header.access_token
            );
            _header.data_length = service::ToHostRepresentation(
                _header.data_length
            );
            _header.user_id = service::ToHostRepresentation(
                _header.user_id
            );
        }
    }

    Packet MakeRequest(DataType _type, std::string_view _resource) {
        Packet result;

        NetworkHeader header;

        header.data_type = _type;
        header.error_code = ErrorCode::kSuccess;

        result.header = header;

        result.data = std::make_shared<char[]>(_resource.size() + 1);

        std::memcpy(result.data.get(), _resource.data(),
            _resource.size());

        result.data[_resource.size()] = '\0';

        return  result;
    }

    Packet MakeRespond(DataType _type,
        const std::shared_ptr<char[]> &_buffer, size_t _size) {

        Packet result;

        NetworkHeader header;

        header.data_type = _type;
        header.data_length = _size;
        header.error_code = ErrorCode::kSuccess;

        result.header = header;
        
        result.data = _buffer;
        
        return result;
    }

    Packet MakeError(DataType _type, std::string_view _message) {
        Packet result = MakeRequest(_type, _message);

        result.header.error_code = ErrorCode::kError;

        return result;
    }
} // namespace cloud_storage::network
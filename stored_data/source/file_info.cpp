#include "file_info.hpp"

#include <cstring>

#include "help_tools.hpp"

namespace cloud_storage::stored_data {
    FileInfo::FileInfo(const network::Packet &_packet) {
        if (_packet.GetHeader().data_type != network::DataType::kFile) {
            // throw
        }

        Deserialize(_packet.GetData());
    }

    std::pair<std::shared_ptr<char[]>, size_t> FileInfo::Serialize() const {
        std::shared_ptr<char[]> result;

        size_t data_size = sizeof(size) + 2 * sizeof(ULARGE_INTEGER) + name.size() + 1;
        
        result.reset(new char[data_size]);

        char *data = result.get();

        std::strcpy(data, name.c_str());
        data += name.size() + 1;
        *reinterpret_cast<decltype(size) *>(data) =
            service::ToNetworkRepresentation(size);
        data += sizeof(size);

        FILETIME filetime_creation_time, filetime_editing_time;
        ULARGE_INTEGER ulint_creation_time, ulint_editing_time;

        SystemTimeToFileTime(&creation_time, &filetime_creation_time);
        SystemTimeToFileTime(&editing_time, &filetime_editing_time);

        ulint_creation_time.HighPart = filetime_creation_time.dwHighDateTime;
        ulint_creation_time.LowPart = filetime_creation_time.dwLowDateTime;
        ulint_editing_time.HighPart = filetime_editing_time.dwHighDateTime;
        ulint_editing_time.LowPart = filetime_editing_time.dwLowDateTime;

        *reinterpret_cast<decltype(ulint_creation_time.QuadPart) *>(data) =
            service::ToNetworkRepresentation(ulint_creation_time.QuadPart);
        data += sizeof(ulint_creation_time.QuadPart);
        *reinterpret_cast<decltype(ulint_editing_time.QuadPart) *>(data) =
            service::ToNetworkRepresentation(ulint_editing_time.QuadPart);

        return { result, data_size };
    }

    FileInfo &FileInfo::Deserialize(const std::shared_ptr<char[]> &_buffer) {
        if (_buffer == nullptr) {
            return *this;
        }

        const char *buffer_ptr = _buffer.get();

        name = std::string(buffer_ptr);
        buffer_ptr += name.size() + 1;
        size = service::ToHostRepresentation(
            *reinterpret_cast<const decltype(size) *>(buffer_ptr));
        buffer_ptr += sizeof(size);

        FILETIME filetime_creation_time, filetime_editing_time;
        ULARGE_INTEGER ulint_creation_time, ulint_editing_time;

        ulint_creation_time.QuadPart = service::ToHostRepresentation(
            *reinterpret_cast<const decltype(
            ulint_creation_time.QuadPart) *>(buffer_ptr));
        buffer_ptr += sizeof(ulint_creation_time.QuadPart);

        ulint_editing_time.QuadPart = service::ToHostRepresentation(
            *reinterpret_cast<const decltype(
            ulint_editing_time.QuadPart) *>(buffer_ptr));

        filetime_creation_time.dwHighDateTime = ulint_creation_time.HighPart;
        filetime_creation_time.dwLowDateTime = ulint_creation_time.LowPart;
        filetime_editing_time.dwHighDateTime = ulint_editing_time.HighPart;
        filetime_editing_time.dwLowDateTime = ulint_editing_time.LowPart;

        FileTimeToSystemTime(&filetime_creation_time, &creation_time);
        FileTimeToSystemTime(&filetime_editing_time, &editing_time);

        return *this;
    }
} // namespace cloud_storage::stored_data
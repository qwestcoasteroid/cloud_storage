#include "file_info.hpp"

#include <cstring>
#include <iostream>

#include "help_tools.hpp"

namespace cloud_storage::stored_data {
    FileInfo::FileInfo(const network::NetworkBuffer &_buffer) {
        Deserialize(_buffer);
    }

    network::NetworkBuffer FileInfo::Serialize() const {
        network::NetworkBuffer result;

        result.data_type = network::DataType::kFileInfo;

        result.length = sizeof(size) + 2 * sizeof(ULARGE_INTEGER) + name.size() + 1;
        
        result.buffer.reset(new char[result.length]);

        char *buffer_ptr = result.buffer.get();

        std::strcpy(buffer_ptr, name.c_str());
        buffer_ptr += name.size() + 1;
        *reinterpret_cast<decltype(size) *>(buffer_ptr) =
            service::ToNetworkRepresentation(size);
        buffer_ptr += sizeof(size);

        FILETIME filetime_creation_time, filetime_editing_time;
        ULARGE_INTEGER ulint_creation_time, ulint_editing_time;

        SystemTimeToFileTime(&creation_time, &filetime_creation_time);
        SystemTimeToFileTime(&editing_time, &filetime_editing_time);

        ulint_creation_time.HighPart = filetime_creation_time.dwHighDateTime;
        ulint_creation_time.LowPart = filetime_creation_time.dwLowDateTime;
        ulint_editing_time.HighPart = filetime_editing_time.dwHighDateTime;
        ulint_editing_time.LowPart = filetime_editing_time.dwLowDateTime;

        *reinterpret_cast<decltype(ulint_creation_time.QuadPart) *>(buffer_ptr) =
            service::ToNetworkRepresentation(ulint_creation_time.QuadPart);
        buffer_ptr += sizeof(ulint_creation_time.QuadPart);
        *reinterpret_cast<decltype(ulint_editing_time.QuadPart) *>(buffer_ptr) =
            service::ToNetworkRepresentation(ulint_editing_time.QuadPart);

        return result;
    }

    FileInfo &FileInfo::Deserialize(const network::NetworkBuffer &_buffer) {
        if (_buffer.buffer == nullptr) {
            return *this;
        }

        if (_buffer.data_type != network::DataType::kFileInfo) {
            // throw
        }

        const char *buffer_ptr = _buffer.buffer.get();

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
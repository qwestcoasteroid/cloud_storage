#include <Windows.h>

#include <string>
#include <cstring>
#include <iostream>
#include <chrono>

#include "protocol.hpp"
#include "network_stream.hpp"
#include "file_part.hpp"

void SendFile(std::string_view _name, uint64_t _size, uint32_t _chunk_size,
    cloud_storage::network::NetworkStream &_stream) {

    HANDLE file = CreateFileA(_name.data(), GENERIC_READ,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    HANDLE file_map = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);

    cloud_storage::messages::FilePart file_part;

    file_part.buffer.reset(new char[_chunk_size]);
    file_part.size = _chunk_size;

    uint64_t sent{};

    LARGE_INTEGER offset{};

    cloud_storage::network::Packet packet;

    do {
        if (_size - sent < _chunk_size) {
            file_part.size = _size - sent;
        }

        LPVOID file_ptr = MapViewOfFile(file_map,
            FILE_MAP_READ, offset.HighPart,
            offset.LowPart, file_part.size
        );
        
        std::memcpy(file_part.buffer.get(), file_ptr, file_part.size);

        AppendPacket(packet, file_part.Serialize());

        try {
            _stream.Write(packet);
        }
        catch (...) {
            UnmapViewOfFile(file_ptr);
            CloseHandle(file_map);
            CloseHandle(file);
            throw;
        }

        sent += file_part.size;

        offset.QuadPart = sent;

        UnmapViewOfFile(file_ptr);

    } while (sent != _size);

    CloseHandle(file_map);
    CloseHandle(file);

    std::cout << "File sent! from [" << GetCurrentThreadId()
        << "] thread." << std::endl;
}

void ReceiveFile(std::string_view _name, uint64_t _size, uint32_t _chunk_size,
    cloud_storage::network::NetworkStream &_stream) {

    HANDLE requested_file = CreateFile(_name.data(),
        GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL
    );

    LARGE_INTEGER file_size{ .QuadPart = static_cast<LONGLONG>(_size) }, offset{};

    SetFilePointerEx(requested_file, file_size, NULL, FILE_BEGIN);

    SetEndOfFile(requested_file);

    HANDLE file_map = CreateFileMapping(requested_file, NULL,
        PAGE_READWRITE, 0, 0, NULL);

    cloud_storage::network::Packet packet;

    cloud_storage::messages::FilePart file_part;

    uint64_t received{};

    do {      
        try {
            packet = _stream.Read();
        }
        catch (...) {
            CloseHandle(file_map);
            CloseHandle(requested_file);
            DeleteFileA(_name.data());
            throw;
        }

        file_part.Deserialize(cloud_storage::network::CreateNetworkBuffer(packet));

        uint64_t buffer_offset = received - _chunk_size *
            (received / _chunk_size);

        LPVOID file_ptr = MapViewOfFile(file_map, FILE_MAP_WRITE,
            offset.HighPart, offset.LowPart, file_part.size);

        std::memcpy(static_cast<char *>(file_ptr) + buffer_offset, file_part.buffer.get(),
            file_part.size
        );

        received += file_part.size;

        offset.QuadPart = received;

        UnmapViewOfFile(file_ptr);

    } while (received != _size);

    CloseHandle(file_map);
    CloseHandle(requested_file);

    std::cout << "File received! in [" << GetCurrentThreadId()
        << "] thread." << std::endl;
}
#include <iostream>

#include "connection.hpp"
#include "file_info.hpp"
#include "network_reading_stream.hpp"
#include "network_writing_stream.hpp"
#include "help_tools.hpp"

#define DEFAULT_FRAGMENT_SIZE ((1 << 10) * 64)

int main() {
    
        std::string file_name;
        std::string dir_name("F:\\");

        std::cout << "Enter file name: ";
        std::getline(std::cin, file_name);
        
        cloud_storage::network::Connection connection("127.0.0.1", "43000");

        connection.Connect();

        cloud_storage::network::NetworkReadingStream reader(connection);
        cloud_storage::network::NetworkWritingStream writer(connection);

        cloud_storage::stored_data::FileInfo req_file;

        req_file.name = file_name;

    try {
        HANDLE file = CreateFile((dir_name + file_name).c_str(),
            GENERIC_READ, 0, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL
        );

        std::cout << dir_name + file_name << std::endl;

        if (file == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("kek");
        }

        LARGE_INTEGER file_size{};

        GetFileSizeEx(file, &file_size);

        req_file.size = file_size.QuadPart;

        auto net_buf = req_file.Serialize();

        cloud_storage::network::Packet pack;

        pack.data = net_buf.buffer;
        pack.header.data_length = net_buf.length;
        pack.header.error_code = cloud_storage::network::ErrorCode::kSuccess;
        pack.header.data_type = cloud_storage::network::DataType::kFileInfo;

        writer.Write(pack);

        pack = reader.Read();

        if (pack.header.error_code ==
            cloud_storage::network::ErrorCode::kError) {

            std::cerr << "Bad request!" << std::endl;
            CloseHandle(file);
            throw std::runtime_error("kek");
        }

        HANDLE file_map = CreateFileMapping(file, NULL,
            PAGE_READONLY, 0, 0, NULL
        );

        LARGE_INTEGER fragment_size{}, offset{};

        fragment_size.QuadPart = DEFAULT_FRAGMENT_SIZE;

        pack.header.data_length = DEFAULT_FRAGMENT_SIZE;
        pack.header.error_code = cloud_storage::network::ErrorCode::kSuccess;

        size_t sent = 0;

        pack.data = std::shared_ptr<char[]>(new char[fragment_size.QuadPart]);

        do {
            try {
                if (req_file.size - sent < DEFAULT_FRAGMENT_SIZE) {
                    fragment_size.QuadPart = req_file.size - sent;
                    pack.header.data_length = fragment_size.QuadPart;
                }

                LPVOID file_ptr = MapViewOfFile(file_map,
                    FILE_MAP_READ, offset.HighPart,
                    offset.LowPart, fragment_size.QuadPart);

                // LPSTR tmp_file_ptr = reinterpret_cast<LPSTR>(file_ptr);
                
                std::memcpy(pack.data.get(), file_ptr, fragment_size.QuadPart);

                writer.Write(pack);

                sent += fragment_size.QuadPart;

                offset.QuadPart = sent;

                UnmapViewOfFile(file_ptr);
            }

            catch (std::exception &e) {
                std::cout << e.what() << std::endl;
                throw e;
            }
            
        } while (sent != req_file.size);

        std::cout << "File sent!" << std::endl;
        
        CloseHandle(file_map);
        CloseHandle(file);
    }
    catch (...) {
        std::cout << "Inner error!\n";
    }

    connection.Disconnect();

    try {
        reader.Read();
    }
    catch (...) {
        std::cout << "Disconnected from " << &connection << std::endl;
    }
    
    return 0;
}
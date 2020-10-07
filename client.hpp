#ifndef CLOUD_STORAGE_CLIENT_HPP_
#define CLOUD_STORAGE_CLIENT_HPP_

#include <string_view>

#include "connection_info.hpp"

class Client {
public:
    Client(std::string_view ip_address, std::string_view port);

    Client(Client &&client) noexcept;
    Client& operator=(Client &&client) noexcept;

    ~Client();

    bool Connect() const;

    const ConnectionInfo &GetConnectionInfo() const;

    friend class Server;
private:
    Client();

    ConnectionInfo m_connection_info;
};

#endif // CLOUD_STORAGE_CLIENT_HPP_
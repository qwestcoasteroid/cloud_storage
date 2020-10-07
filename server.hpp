#ifndef CLOUD_STORAGE_SERVER_HPP_
#define CLOUD_STORAGE_SERVER_HPP_

#include <string_view>

#include "connection_info.hpp"
#include "client.hpp"

class Server {
public:
    Server(std::string_view port);

    ~Server();

    const ConnectionInfo &GetConnectionInfo() const;

    Client Accept() const;
private:
    ConnectionInfo m_connection_info; // Redundant...
};

#endif // CLOUD_STORAGE_SERVER_HPP_
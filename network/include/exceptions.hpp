#ifndef CLOUD_STORAGE_NETWORK_EXCEPTIONS_H_
#define CLOUD_STORAGE_NETWORK_EXCEPTIONS_H_

#include <exception>

namespace cloud_storage::network::exceptions {
    class SocketError : public std::exception {
    public:
        inline explicit SocketError(int _error_code) noexcept;

        inline int GetErrorCode() const noexcept;

    protected:
        int error_code_{};
    };

    class NetworkError : std::exception {};

    class ConnectionClosed : public NetworkError {};

    class UnknownProtocol : public NetworkError {};

    SocketError::SocketError(int _error_code) noexcept
        : error_code_(_error_code) {}

    int SocketError::GetErrorCode() const noexcept {
        return error_code_;
    }
} // namespace cloud_storage::network

#endif // CLOUD_STORAGE_NETWORK_EXCEPTIONS_H_
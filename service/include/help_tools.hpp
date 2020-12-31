#ifndef CLOUD_STORAGE_SERVICE_HELP_TOOLS_HPP_
#define CLOUD_STORAGE_SERVICE_HELP_TOOLS_HPP_

#include <ws2tcpip.h>

namespace cloud_storage::service {
    static auto IsLittleEndian = []() -> bool {
        static constexpr int value{ 1 };
        static bool result{
            value == *reinterpret_cast<const char *>(&value)
        };
        return result;
    };

    class ByteOrderSwitcher {
    public:
        ByteOrderSwitcher() = delete;

        template<typename T>
        static T Switch(T _value);
    };

    template<typename T>
    T ByteOrderSwitcher::Switch<T>(T _value) {
        if (!IsLittleEndian()) {
            return _value;
        }

        T result{};
        const unsigned char *byte{
            reinterpret_cast<const unsigned char *>(&_value) + sizeof(T) - 1
        };
        unsigned char *result_ptr{
            reinterpret_cast<unsigned char *>(&result)
        };

        for (size_t i = 0; i < sizeof(T); ++i, --byte) {
            *(result_ptr + i) = *byte;
        }

        return result;
    }
} // namespace cloud_storage::service

#endif // CLOUD_STORAGE_SERVICE_HELP_TOOLS_HPP_
#ifndef CLOUD_STORAGE_SERVICE_HELP_TOOLS_HPP_
#define CLOUD_STORAGE_SERVICE_HELP_TOOLS_HPP_

#include <ws2tcpip.h>

#include <type_traits>

namespace cloud_storage::service {
    using uint16_t = unsigned short;
    using uint32_t = unsigned int;
    using uint64_t = unsigned long long;

    uint64_t htonll(uint64_t value);

    uint64_t ntohll(uint64_t value);

    template<typename T>
    std::decay_t<T> ToHostRepresentation(T &&_arg) {
        using ArgType = std::remove_reference_t<T>;

        constexpr size_t size = sizeof(ArgType);

        static_assert(size == sizeof(uint16_t) || size ==
            sizeof(uint32_t) || size == sizeof(uint64_t),
            "Unconvertable type!");

        if constexpr (size == sizeof(uint16_t)) {
            return static_cast<ArgType>(ntohs(static_cast<uint16_t>(_arg)));
        }
        else if constexpr (size == sizeof(uint32_t)) {
            return static_cast<ArgType>(ntohl(static_cast<uint32_t>(_arg)));
        }
        else if constexpr (size == sizeof(uint64_t)) {
            return static_cast<ArgType>(ntohll(static_cast<uint64_t>(_arg)));
        }
    }

    template<typename T>
    std::decay_t<T> ToNetworkRepresentation(T &&_arg) {
        using ArgType = std::remove_reference_t<T>;

        constexpr size_t size = sizeof(ArgType);

        static_assert(size == sizeof(uint16_t) || size ==
            sizeof(uint32_t) || size == sizeof(uint64_t),
            "Unconvertable type!");

        if constexpr (size == sizeof(uint16_t)) {
            return static_cast<ArgType>(htons(static_cast<uint16_t>(_arg)));
        }
        else if constexpr (size == sizeof(uint32_t)) {
            return static_cast<ArgType>(htonl(static_cast<uint32_t>(_arg)));
        }
        else if constexpr (size == sizeof(uint64_t)) {
            return static_cast<ArgType>(htonll(static_cast<uint64_t>(_arg)));
        }
    }
} // namespace cloud_storage::service

#endif // CLOUD_STORAGE_SERVICE_HELP_TOOLS_HPP_
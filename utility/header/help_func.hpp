#ifndef CLOUD_STORAGE_UTILITY_HELP_FUNC_HPP_
#define CLOUD_STORAGE_UTILITY_HELP_FUNC_HPP_

namespace cloud_storage::utility {
    using uint64_t = unsigned long long;
    using uint32_t = unsigned int;

    uint64_t htonll(uint64_t value);

    uint64_t ntohll(uint64_t value);
} // namespace cloud_storage::utility



#endif // CLOUD_STORAGE_UTILITY_HELP_FUNC_HPP_
#ifndef CLOUD_STORAGE_SERVICE_SERIALIZABLE_HPP_
#define CLOUD_STORAGE_SERVICE_SERIALIZABLE_HPP_

#include <memory>

namespace cloud_storage::service {
    class Serializable {
    public:
        virtual std::pair<std::shared_ptr<char[]>, size_t> Serialize() const = 0;
        virtual Serializable &Deserialize(const std::shared_ptr<char[]> & = nullptr) = 0;
    };
} // namespace cloud_storage::service

#endif // CLOUD_STORAGE_SERVICE_SERIALIZABLE_HPP_
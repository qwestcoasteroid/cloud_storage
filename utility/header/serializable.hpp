#ifndef CLOUD_STORAGE_UTILITY_SERIALIZABLE_HPP_
#define CLOUD_STORAGE_UTILITY_SERIALIZABLE_HPP_

namespace cloud_storage::utility {
    class Serializable {
    public:
        virtual void Serialize() const = 0;
        virtual void Deserialize(const void *) = 0;
    };
} // namespace cloud_storage::utility

#endif // CLOUD_STORAGE_UTILITY_SERIALIZABLE_HPP_
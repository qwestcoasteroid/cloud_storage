#ifndef CLOUD_STORAGE_NETWORK_NETWORK_OBJECT_HPP
#define CLOUD_STORAGE_NETWORK_NETWORK_OBJECT_HPP

namespace cloud_storage::network {
    class NetworkObject {
    public:
        NetworkObject();
        virtual ~NetworkObject();
    private:
        static inline unsigned long count_{ 0 };
    };
}

#endif // CLOUD_STORAGE_NETWORK_NETWORK_OBJECT_HPP
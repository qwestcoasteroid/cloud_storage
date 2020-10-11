#include "network_object.hpp"

#include "library.hpp"

namespace cloud_storage::network {
    NetworkObject::NetworkObject() {
        if (count_ == 0) {
            Library::Initialize();
        }

        ++count_;
    }

    NetworkObject::~NetworkObject() {
        --count_;

        if (count_ == 0) {
            Library::Deinitialize();
        }
    }
} // namespace cloud_storage::network
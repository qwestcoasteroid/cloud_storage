#include "storageinfo.h"

std::shared_ptr<StorageInfo> StorageInfo::get() {
    static std::shared_ptr<StorageInfo> instance(new StorageInfo);

    return instance;
}

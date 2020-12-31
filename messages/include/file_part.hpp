#ifndef CLOUD_STORAGE_MESSAGES_FILE_PART_H_
#define CLOUD_STORAGE_MESSAGES_FILE_PART_H_

#include <cstdint>
#include <memory>

#include "serializable.hpp"
#include "protocol.hpp"

namespace cloud_storage::messages {
    class FilePart : public service::Serializable {
    public:
        inline FilePart() noexcept {}
        FilePart(const network::NetworkBuffer &_packet);

        network::NetworkBuffer Serialize() const override;
        FilePart &Deserialize(const network::NetworkBuffer &_buffer) override;

        uint32_t operation_id{};
        uint32_t size{};
        std::shared_ptr<char[]> buffer;
    };
} // namespace cloud_storage::messages

#endif // CLOUD_STORAGE_MESSAGES_FILE_PART_H_
#include "protocol.hpp"

#include <ws2tcpip.h>

namespace cloud_storage::network {
    void Header::HostRepresentation() {
        data_length = htonl(data_length);
        data_type = static_cast<DataType>(
            htonl(static_cast<unsigned long>(data_type)));
    }

    void Header::NetworkRepresentation() {
        data_length = ntohl(data_length);
        data_type = static_cast<DataType>(
            ntohl(static_cast<unsigned long>(data_type)));
    }

    TransmissionUnit::TransmissionUnit(TransmissionUnit &&object) noexcept :
        header(object.header), data(std::move(object.data)) {}
} // namespace cloud_storage::network
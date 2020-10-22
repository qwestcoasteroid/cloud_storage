#include "protocol.hpp"

namespace cloud_storage::network {
    TransmissionUnit::TransmissionUnit(TransmissionUnit &&unit) noexcept :
        header(unit.header), data(std::move(unit.data)) {}

    TransmissionUnit &TransmissionUnit::operator=
        (TransmissionUnit &&unit) noexcept {
        
        if (&unit == this) {
            return *this;
        }

        header = unit.header;
        data = std::move(unit.data);
        
        return *this;
    }
} // namespace cloud_storage::network
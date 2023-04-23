#include "NoReorder.h"


namespace placing_strategy::partition_policy {

unsigned NoReorder::Partition(std::vector<const Machine*>& machines) {
    return static_cast<unsigned>(machines.size());
}

} // namespace placing_strategy::partition_policy

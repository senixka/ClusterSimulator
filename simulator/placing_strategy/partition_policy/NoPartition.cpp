#include "NoPartition.h"


namespace placing_strategy::partition_policy {

size_t NoPartition::Partition(std::vector<const Machine*>& machines) {
    return machines.size();
}

} // namespace placing_strategy::partition_policy

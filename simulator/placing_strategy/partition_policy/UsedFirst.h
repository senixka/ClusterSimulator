#pragma once

#include "../../MachineManager.h"


namespace placing_strategy::partition_policy {

struct UsedFirst {
    static size_t Partition(std::vector<const Machine*>& machines);
};

} // namespace placing_strategy::partition_policy

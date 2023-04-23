#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"

#include <random>


namespace placing_strategy {

template<class PartitionPolicy>
struct Random : public IPlacingStrategy {
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        unsigned split = PartitionPolicy::Partition(machines);
        if (split == 0) {
            split = static_cast<unsigned>(machines.size());
        }

        return machines[std::rand() % split]->machineIndex_;
    }
};

} // namespace placing_strategy

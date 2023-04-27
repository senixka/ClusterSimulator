#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"


namespace placing_strategy {

template<class PartitionPolicy>
struct MinMachineIndex : public IPlacingStrategy {
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        size_t split = PartitionPolicy::Partition(machines);
        if (split == 0) {
            split = machines.size();
        }

        unsigned bestMachineIndex{machines[0]->machineIndex_};

        for (size_t i = 1; i < split; ++i) {
            const Machine* machine = machines[i];

            if (machine->machineIndex_ < bestMachineIndex) {
                bestMachineIndex = machine->machineIndex_;
            }
        }

        return bestMachineIndex;
    }
};

} // namespace placing_strategy

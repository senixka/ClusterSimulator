#pragma once

#include "IPlacingStrategy.h"
#include "../Defines.h"
#include "../Macro.h"


namespace placing_strategy {

static_assert(static_cast<uint64_t>(MACHINE_MAX_POSSIBLE_CPU) *
              static_cast<uint64_t>(MACHINE_MAX_POSSIBLE_MEMORY) < static_cast<uint64_t>(UINT32_MAX));

template<class PartitionPolicy>
struct MaxVolumeBefore : public IPlacingStrategy {
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        size_t split = PartitionPolicy::Partition(machines);
        if (split == 0) {
            split = machines.size();
        }

        unsigned bestMachineIndex{machines[0]->machineIndex_};
        unsigned maxVolume{machines[0]->availableCpu_ * machines[0]->availableMemory_}, currentVolume;

        for (size_t i = 1; i < split; ++i) {
            currentVolume = machines[i]->availableCpu_ * machines[i]->availableMemory_;

            if (currentVolume > maxVolume) {
                maxVolume = currentVolume;
                bestMachineIndex = machines[i]->machineIndex_;
            }
        }

        return bestMachineIndex;
    }
};

} // namespace placing_strategy

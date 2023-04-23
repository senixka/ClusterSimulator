#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"


namespace placing_strategy {

template<class PartitionPolicy>
struct MaxVolume : public IPlacingStrategy {
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        unsigned split = PartitionPolicy::Partition(machines);
        if (split == 0) {
            split = static_cast<unsigned>(machines.size());
        }

        unsigned bestMachineIndex{0};
        double maxVolume{-10};

        for (unsigned i = 0; i < split; ++i) {
            const Machine* machine = machines[i];

            const auto mResources = machine->GetResources();
            const auto mVolume = std::get<0>(mResources) * std::get<1>(mResources) * std::get<2>(mResources);

            if (mVolume > maxVolume) {
                maxVolume = mVolume;
                bestMachineIndex = machine->machineIndex_;
            }
        }

        return bestMachineIndex;
    }
};

} // namespace placing_strategy

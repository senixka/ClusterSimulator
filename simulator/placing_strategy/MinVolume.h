#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"


namespace placing_strategy {

template<class PriorityReorder>
class MinVolume : public IPlacingStrategy {
public:
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        unsigned split = reorder_.Reorder(machines);
        if (split == 0) {
            split = static_cast<unsigned>(machines.size());
        }

        unsigned bestMachineIndex{0};
        double minVolume{10};

        for (unsigned i = 0; i < split; ++i) {
            const Machine* machine = machines[i];

            const auto mResources = machine->GetResources();
            const auto mVolume = std::get<0>(mResources) * std::get<1>(mResources) * std::get<2>(mResources);

            if (mVolume < minVolume) {
                minVolume = mVolume;
                bestMachineIndex = machine->machineIndex_;
            }
        }

        return bestMachineIndex;
    }

private:
    PriorityReorder reorder_;
};

} // namespace placing_strategy

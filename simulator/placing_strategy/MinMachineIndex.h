#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"


namespace placing_strategy {

template<class PriorityReorder>
class MinMachineIndex : public IPlacingStrategy {
public:
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        unsigned split = reorder_.Reorder(machines);
        if (split == 0) {
            split = static_cast<unsigned>(machines.size());
        }

        unsigned bestMachineIndex{machines[0]->machineIndex_};

        for (unsigned i = 0; i < split; ++i) {
            const Machine* machine = machines[i];

            if (machine->machineIndex_ < bestMachineIndex) {
                bestMachineIndex = machine->machineIndex_;
            }
        }

        return bestMachineIndex;
    }

private:
    PriorityReorder reorder_;
};

} // namespace placing_strategy
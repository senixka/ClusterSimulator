#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"

#include <random>


namespace placing_strategy {

template<class PriorityReorder>
class Random : public IPlacingStrategy {
public:
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* /*task*/) override {
        ASSERT(!machines.empty());

        unsigned split = reorder_.Reorder(machines);
        if (split == 0) {
            split = static_cast<unsigned>(machines.size());
        }

        return machines[std::rand() % split]->machineIndex_;
    }

private:
    PriorityReorder reorder_;
};

} // namespace placing_strategy

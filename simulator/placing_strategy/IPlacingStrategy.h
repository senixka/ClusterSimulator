#pragma once

#include "../Task.h"
#include "../MachineManager.h"


class IPlacingStrategy {
public:
    virtual unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* task) = 0;

    virtual ~IPlacingStrategy() = default;
};

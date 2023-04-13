#pragma once

#include "../Task.h"
#include "../MachineManager.h"


class IPlacingStrategy {
public:
    virtual size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) = 0;

    virtual ~IPlacingStrategy() = default;
};

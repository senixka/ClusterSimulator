#pragma once

#include "IPlacingStrategy.h"


class Random : public IPlacingStrategy {
public:
    size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) override;
};

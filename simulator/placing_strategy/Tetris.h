#pragma once

#include "IPlacingStrategy.h"


class Tetris : public IPlacingStrategy {
public:
    size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) override;
};

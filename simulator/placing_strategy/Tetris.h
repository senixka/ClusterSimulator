#ifndef CLUSTERSIMULATOR_TETRIS_H
#define CLUSTERSIMULATOR_TETRIS_H

#include "IPlacingStrategy.h"


class Tetris : public IPlacingStrategy {
public:
    size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) override;
};


#endif //CLUSTERSIMULATOR_TETRIS_H

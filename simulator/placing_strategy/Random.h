#ifndef CLUSTERSIMULATOR_RANDOM_H
#define CLUSTERSIMULATOR_RANDOM_H

#include "IPlacingStrategy.h"


class Random : public IPlacingStrategy {
public:
    size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) override;
};


#endif //CLUSTERSIMULATOR_RANDOM_H

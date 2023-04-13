#ifndef CLUSTERSIMULATOR_IPLACINGSTRATEGY_H
#define CLUSTERSIMULATOR_IPLACINGSTRATEGY_H

#include "../Task.h"
#include "../MachineManager.h"


class IPlacingStrategy {
public:
    virtual size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) = 0;

    virtual ~IPlacingStrategy() = default;
};


#endif //CLUSTERSIMULATOR_IPLACINGSTRATEGY_H

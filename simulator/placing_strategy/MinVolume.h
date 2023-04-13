#ifndef CLUSTERSIMULATOR_MINVOLUME_H
#define CLUSTERSIMULATOR_MINVOLUME_H

#include "IPlacingStrategy.h"


class MinVolume : public IPlacingStrategy {
public:
    size_t BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) override;
};

#endif //CLUSTERSIMULATOR_MINVOLUME_H

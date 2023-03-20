#ifndef CLUSTERSIMULATOR_MACHINE_H
#define CLUSTERSIMULATOR_MACHINE_H

#include "Task.h"


class Machine {
public:
    double cpuCapacity{0.7};
    double memoryCapacity{0.7};
    double diskSpaceCapacity{0.5};


    bool IsTaskPlaceable(const Task& task) const;

    void PlaceTask(const Task& task);

    void RemoveTask(const Task& task);
};


#endif //CLUSTERSIMULATOR_MACHINE_H

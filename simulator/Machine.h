#ifndef CLUSTERSIMULATOR_MACHINE_H
#define CLUSTERSIMULATOR_MACHINE_H

#include "Task.h"


class Machine {
public:
    double cpuCapacity{1};
    double memoryCapacity{1};
    double diskSpaceCapacity{1};


    bool IsTaskPlaceable(const Task& task) const;

    void PlaceTask(const Task& task);

    void RemoveTask(const Task& task);
};


#endif //CLUSTERSIMULATOR_MACHINE_H

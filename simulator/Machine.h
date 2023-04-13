#pragma once

#include "Task.h"


class Machine {
public:
    double currentCpu{0};
    double currentMemory{0};
    double currentDiskSpace{0};


    bool IsTaskPlaceable(const Task& task) const;

    void PlaceTask(const Task& task);

    void RemoveTask(const Task& task);
};

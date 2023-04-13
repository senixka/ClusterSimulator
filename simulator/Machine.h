#pragma once

#include "Task.h"


class Machine {
public:
    bool IsTaskPlaceable(const Task& task) const;
    void PlaceTask(const Task& task);
    void RemoveTask(const Task& task);

public:
    double currentCpu_{0};
    double currentMemory_{0};
    double currentDiskSpace_{0};
};

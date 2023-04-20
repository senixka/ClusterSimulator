#pragma once

#include "Task.h"

#include <cstddef>


class Machine {
public:
//    explicit Machine(size_t machineIndex, size_t currentTaskCount,
//                     double currentCpu, double currentMemory, double currentDiskSpace);

    bool IsTaskPlaceable(const Task& task) const;
    std::tuple<double, double, double> GetResources() const;

    void PlaceTask(const Task& task);
    void RemoveTask(const Task& task);

public:
    const unsigned machineIndex_{0};
    size_t currentTaskCount_{0};

    double currentCpu_{0};
    double currentMemory_{0};
    double currentDiskSpace_{0};
};

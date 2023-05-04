#pragma once

#include "Task.h"
#include "Macro.h"
#include "Defines.h"

#include <cstddef>


class Machine {
public:
    explicit Machine(unsigned machineIndex, ResourceT availableCpu, ResourceT availableMemory)
             : machineIndex_(machineIndex), availableCpu_(availableCpu), availableMemory_(availableMemory) {
        ASSERT(availableCpu_ <= MACHINE_MAX_POSSIBLE_CPU);
        ASSERT(availableMemory_ <= MACHINE_MAX_POSSIBLE_MEMORY);
    }

    inline bool IsTaskPlaceable(const Task& task) const {
        return task.cpuRequest_ <= availableCpu_ &&
               task.memoryRequest_ <= availableMemory_;
    }

    inline void PlaceTask(const Task& task) {
        ASSERT(IsTaskPlaceable(task) && currentTaskCount_ < UINT32_MAX);

        ++currentTaskCount_;

        availableCpu_ -= task.cpuRequest_;
        availableMemory_ -= task.memoryRequest_;
    }

    inline void RemoveTask(const Task& task) {
        ASSERT(currentTaskCount_ > 0);

        --currentTaskCount_;

        availableCpu_ += task.cpuRequest_;
        availableMemory_ += task.memoryRequest_;
    }

public:
    const unsigned machineIndex_{0};
    unsigned currentTaskCount_{0};

    ResourceT availableCpu_{0};
    ResourceT availableMemory_{0};
};

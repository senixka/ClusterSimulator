#pragma once

#include "Task.h"
#include "Macro.h"
#include "Defines.h"

#include <cstddef>


class Machine {
public:
    explicit Machine(unsigned machineIndex, size_t currentTaskCount,
                     unsigned availableCpu, unsigned availableMemory)
             : machineIndex_(machineIndex), currentTaskCount_(currentTaskCount),
               availableCpu_(availableCpu), availableMemory_(availableMemory) {
        ASSERT(availableCpu_ <= MACHINE_MAX_POSSIBLE_CPU);
        ASSERT(availableMemory_ <= MACHINE_MAX_POSSIBLE_MEMORY);
    }

    inline bool IsTaskPlaceable(const Task& task) const {
        return task.cpuRequest_ <= availableCpu_ &&
               task.memoryRequest_ <= availableMemory_;
    }

    inline void PlaceTask(const Task& task) {
        ASSERT(IsTaskPlaceable(task));

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
    size_t currentTaskCount_{0};

    unsigned availableCpu_{0};
    unsigned availableMemory_{0};
};

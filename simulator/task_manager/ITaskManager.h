#pragma once

#include "../Defines.h"
#include "../Task.h"

#include <cstdlib>


// Correct SumTaskEstimateTime() return type
static_assert(2 * sizeof(BoundedTimeT) == sizeof(uint64_t));


struct ITaskManager {
    virtual void NewTasks(unsigned nTask,
                          BoundedTimeT estimate, ResourceT cpuRequest, ResourceT memoryRequest, JobIdT jobID) = 0;

    virtual Task* GetTask() = 0;
    virtual void ReturnTask(Task* task, bool isScheduled) = 0;

    virtual void NewSchedulingCycle() = 0;
    virtual bool IsThereSomethingElse() = 0;

    virtual unsigned TaskCount() = 0;
    virtual void Sort() = 0;

    virtual uint64_t SumTaskEstimateTime() = 0;
    virtual BoundedTimeT MaxTaskEstimateTime() = 0;

    virtual ~ITaskManager() = default;
};

#pragma once

#include "../Task.h"

#include <cstdlib>


struct ITaskManager {
    virtual void PutTask(Task* task) = 0;

    virtual Task* GetTask() = 0;
    virtual void ReturnTask(Task* task, bool isScheduled) = 0;

    virtual void NewSchedulingCycle() = 0;

    virtual size_t TaskCount() = 0;
    virtual void Sort() = 0;

    virtual unsigned __int128 SumTaskEstimateTime() = 0;
    virtual uint64_t MaxTaskEstimateTime() = 0;

    virtual ~ITaskManager() = default;
};

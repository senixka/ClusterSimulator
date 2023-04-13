#ifndef CLUSTERSIMULATOR_ITASKMANAGER_H
#define CLUSTERSIMULATOR_ITASKMANAGER_H

#include "../Task.h"

#include <cstdlib>


class ITaskManager {
public:
    virtual void PutTask(Task* task) = 0;

    virtual Task* GetTask() = 0;
    virtual size_t TaskCount() = 0;
    virtual uint64_t MaxTaskEstimateTime() = 0;

    virtual void ReturnTask(Task* task) = 0;

    virtual ~ITaskManager() = default;
};


#endif //CLUSTERSIMULATOR_ITASKMANAGER_H

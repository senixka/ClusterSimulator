#pragma once

#include "Task.h"
#include "Defines.h"
#include "ClusterEvent.h"
#include "task_manager/ITaskManager.h"
#include "task_manager/FactoryTaskManager.h"

#include <list>
#include <string>
#include <iostream>


class Job : public ClusterEvent {
public:
    Job() = default;
    ~Job();

    static void PrepareMemoryPool(unsigned nJob);
    static Job* New(TaskManagerType taskManagerType, std::istream& in, JobIdT jobId);
    static void Delete(Job* ptr);

public:
    JobIdT jobId_{0};
    BoundedTimeT jobTime_{0};

    ITaskManager* taskManager_{nullptr};
};

#pragma once

#include "Task.h"
#include "ClusterEvent.h"
#include "task_manager/ITaskManager.h"
#include "task_manager/FactoryTaskManager.h"

#include <list>
#include <string>
#include <iostream>


class Job : public ClusterEvent {
public:
    uint64_t jobTime{0};
    uint64_t jobID{0};
    std::string user;
    ITaskManager* taskManager{nullptr};

    Job(TaskManagerType taskManagerType, std::istream& in);
};

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
    explicit Job(TaskManagerType taskManagerType, std::istream& in);
    ~Job();

public:
    uint64_t jobTime_{0};
    uint64_t jobID_{0};
    std::string user_;
    ITaskManager* taskManager_{nullptr};
};

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
    explicit Job(TaskManagerType taskManagerType, std::istream& in, unsigned jobID);
    ~Job();

public:
    ITaskManager* taskManager_{nullptr};

    uint64_t jobTime_{0};
    const unsigned jobID_{0};
};

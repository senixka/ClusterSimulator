#pragma once

#include "ITaskManager.h"
#include "../Task.h"

#include <list>


class InJobOrder : public ITaskManager {
public:
    void PutTask(Task* task) override;

    Task* GetTask() override;
    void ReturnTask(Task* task) override;

    size_t TaskCount() override;
    uint64_t MaxTaskEstimateTime() override;

    ~InJobOrder();

private:
    std::list<Task*> tasks;
};

#pragma once

#include "ITaskManager.h"
#include "../Task.h"

#include <list>


namespace task_manager {

class RoundRobinNB : public ITaskManager {
public:
    void PutTask(Task* task) override;

    Task* GetTask() override;
    void ReturnTask(Task* task, bool isScheduled) override;

    void NewSchedulingCycle() override;

    size_t TaskCount() override;
    void Sort() override;

    unsigned __int128 SumTaskEstimateTime() override;
    uint64_t MaxTaskEstimateTime() override;

    ~RoundRobinNB();

private:
    std::list<Task*> tasks_;
    typename std::list<Task*>::iterator it_;
    unsigned __int128 sumTaskEstimateTime_{0};
};

} // namespace task_manager

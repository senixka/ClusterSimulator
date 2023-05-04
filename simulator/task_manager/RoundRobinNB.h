#pragma once

#include "ITaskManager.h"
#include "../Task.h"

#include <list>
#include <vector>


namespace task_manager {

class RoundRobinNB : public ITaskManager {
public:
    void NewTasks(unsigned kTask,
                  BoundedTimeT estimate, ResourceT cpuRequest, ResourceT memoryRequest, JobIdT jobID) override;

    Task* GetTask() override;
    void ReturnTask(Task* task, bool isScheduled) override;

    void NewSchedulingCycle() override;
    bool IsThereSomethingElse() override;

    unsigned TaskCount() override;
    void Sort() override;

    uint64_t SumTaskEstimateTime() override;
    BoundedTimeT MaxTaskEstimateTime() override;

    ~RoundRobinNB();

private:
    std::list<std::vector<Task*>> tasks_;
    typename decltype(tasks_)::iterator it_;

    unsigned taskCount_{0};
    uint64_t sumTaskEstimateTime_{0};

    size_t currentIter_{0};
    size_t currentSize_{0};
};

} // namespace task_manager

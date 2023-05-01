#pragma once

#include "ITaskManager.h"
#include "../Task.h"

#include <list>
#include <vector>


namespace task_manager {

class RoundRobinNB : public ITaskManager {
public:
    void NewTasks(size_t kTask,
                  uint64_t estimate, unsigned cpuRequest, unsigned memoryRequest, unsigned jobID) override;

    Task* GetTask() override;
    void ReturnTask(Task* task, bool isScheduled) override;

    void NewSchedulingCycle() override;
    bool IsThereSomethingElse() override;

    size_t TaskCount() override;
    void Sort() override;

    unsigned __int128 SumTaskEstimateTime() override;
    uint64_t MaxTaskEstimateTime() override;

    ~RoundRobinNB();

private:
    std::list<std::vector<Task*>> tasks_;
    typename decltype(tasks_)::iterator it_;

    size_t currentIter_{0};
    size_t currentSize_{0};

    size_t taskCount_{0};
    unsigned __int128 sumTaskEstimateTime_{0};
};

} // namespace task_manager

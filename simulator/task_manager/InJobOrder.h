#pragma once

#include "ITaskManager.h"
#include "../Task.h"

#include <list>


namespace task_manager {

class InJobOrder : public ITaskManager {
public:
    void PutTask(Task* task) override;

    Task* GetTask() override;
    void ReturnTask(Task* task) override;

    size_t TaskCount() override;
    void Sort() override;

    unsigned __int128 SumTaskEstimateTime() override;
    uint64_t MaxTaskEstimateTime() override;

    ~InJobOrder();

private:
    std::list<Task*> tasks_;
    unsigned __int128 sumTaskEstimateTime_{0};
};

} // namespace task_manager

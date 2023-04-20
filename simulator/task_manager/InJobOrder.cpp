#include "InJobOrder.h"

#include "../BoundedTime.h"
#include "../Macro.h"


namespace task_manager {

void InJobOrder::PutTask(Task* task) {
    tasks_.push_back(task);

    sumTaskEstimateTime_ = BoundedSum(sumTaskEstimateTime_, task->estimate_);
}

Task* InJobOrder::GetTask() {
    ASSERT(!tasks_.empty());

    Task* task = tasks_.front();
    tasks_.pop_front();

    sumTaskEstimateTime_ -= task->estimate_;
    return task;
}

void InJobOrder::ReturnTask(Task* task) {
    tasks_.push_front(task);

    sumTaskEstimateTime_ = BoundedSum(sumTaskEstimateTime_, task->estimate_);
}

size_t InJobOrder::TaskCount() {
    return tasks_.size();
}

void InJobOrder::Sort() {
}

uint64_t InJobOrder::SumTaskEstimateTime() {
    return sumTaskEstimateTime_;
}

uint64_t InJobOrder::MaxTaskEstimateTime() {
    ASSERT(!tasks_.empty());

    uint64_t maxTaskEstimateTime = tasks_.front()->estimate_;
    for (const auto& task : tasks_) {
        if (maxTaskEstimateTime < task->estimate_) {
            maxTaskEstimateTime = task->estimate_;
        }
    }

    return maxTaskEstimateTime;
}

InJobOrder::~InJobOrder() {
    for (Task* task : tasks_) {
        delete task;
    }
}

} // namespace task_manager

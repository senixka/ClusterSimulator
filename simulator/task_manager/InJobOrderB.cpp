#include "InJobOrderB.h"

#include "../Macro.h"


namespace task_manager {

void InJobOrderB::PutTask(Task* task) {
    tasks_.push_back(task);

    sumTaskEstimateTime_ += task->estimate_;
}

Task* InJobOrderB::GetTask() {
    ASSERT(!tasks_.empty());

    Task* task = tasks_.front();
    tasks_.pop_front();

    sumTaskEstimateTime_ -= task->estimate_;
    return task;
}

void InJobOrderB::ReturnTask(Task* task) {
    tasks_.push_front(task);

    sumTaskEstimateTime_ += task->estimate_;
}

size_t InJobOrderB::TaskCount() {
    return tasks_.size();
}

void InJobOrderB::Sort() {
}

unsigned __int128 InJobOrderB::SumTaskEstimateTime() {
    return sumTaskEstimateTime_;
}

uint64_t InJobOrderB::MaxTaskEstimateTime() {
    ASSERT(!tasks_.empty());

    uint64_t maxTaskEstimateTime = tasks_.front()->estimate_;
    for (const auto& task : tasks_) {
        if (maxTaskEstimateTime < task->estimate_) {
            maxTaskEstimateTime = task->estimate_;
        }
    }

    return maxTaskEstimateTime;
}

InJobOrderB::~InJobOrderB() {
    for (Task* task : tasks_) {
        delete task;
    }
}

} // namespace task_manager

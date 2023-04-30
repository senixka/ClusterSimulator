#include "RoundRobinNB.h"

#include "../Macro.h"


namespace task_manager {

void RoundRobinNB::PutTask(Task* task) {
    tasks_.push_back(task);
    sumTaskEstimateTime_ += task->estimate_;
}

Task* RoundRobinNB::GetTask() {
    ASSERT(it_ != tasks_.end());
    return *it_;
}

void RoundRobinNB::ReturnTask(Task* task, bool isScheduled) {
    if (isScheduled) {
        it_ = tasks_.erase(it_);
        sumTaskEstimateTime_ -= task->estimate_;
    } else {
        ++it_;
    }

    if (it_ == tasks_.end()) {
        it_ = tasks_.begin();
    }
}

void RoundRobinNB::NewSchedulingCycle() {
}

size_t RoundRobinNB::TaskCount() {
    return tasks_.size();
}

void RoundRobinNB::Sort() {
    it_ = tasks_.begin();
}

unsigned __int128 RoundRobinNB::SumTaskEstimateTime() {
    return sumTaskEstimateTime_;
}

uint64_t RoundRobinNB::MaxTaskEstimateTime() {
    ASSERT(!tasks_.empty());

    uint64_t maxTaskEstimateTime = tasks_.front()->estimate_;
    for (const auto& task : tasks_) {
        if (maxTaskEstimateTime < task->estimate_) {
            maxTaskEstimateTime = task->estimate_;
        }
    }

    return maxTaskEstimateTime;
}

RoundRobinNB::~RoundRobinNB() {
    for (Task* task : tasks_) {
        delete task;
    }
}

} // namespace task_manager

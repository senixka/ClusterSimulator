#include "RoundRobinNB.h"

#include "../Macro.h"


namespace task_manager {

void RoundRobinNB::NewTasks(size_t kTask,
                            uint64_t estimate, unsigned cpuRequest, unsigned memoryRequest, unsigned jobID) {
    ASSERT(kTask != 0);

    taskCount_ += kTask;
    tasks_.push_back(std::move(std::vector<Task*>(kTask)));

    for (size_t i = 0; i < kTask; ++i) {
        Task* task = new Task(estimate, cpuRequest, memoryRequest, jobID);

        tasks_.back()[i] = task;
        sumTaskEstimateTime_ += task->estimate_;
    }
}

Task* RoundRobinNB::GetTask() {
    ASSERT(it_ != tasks_.end() && !it_->empty());
    return it_->back();
}

void RoundRobinNB::ReturnTask(Task* task, bool isScheduled) {
    if (isScheduled) {
        it_->pop_back();
        sumTaskEstimateTime_ -= task->estimate_;
        --taskCount_;

        if (it_->empty()) {
            it_ = tasks_.erase(it_);
            ++currentIter_;
        }
    } else {
        ++it_;
        ++currentIter_;
    }

    if (it_ == tasks_.end()) {
        it_ = tasks_.begin();
    }
}

void RoundRobinNB::NewSchedulingCycle() {
    currentIter_ = 0;
    currentSize_ = TaskCount();
}

bool RoundRobinNB::IsThereSomethingElse() {
    return currentIter_ < currentSize_;
}

size_t RoundRobinNB::TaskCount() {
    return taskCount_;
}

void RoundRobinNB::Sort() {
    it_ = tasks_.begin();

    currentIter_ = 0;
    currentSize_ = TaskCount();
}

unsigned __int128 RoundRobinNB::SumTaskEstimateTime() {
    return sumTaskEstimateTime_;
}

uint64_t RoundRobinNB::MaxTaskEstimateTime() {
    ASSERT(!tasks_.empty());

    uint64_t maxTaskEstimateTime{0};
    for (const auto& entry : tasks_) {
        maxTaskEstimateTime = std::max(maxTaskEstimateTime, entry.front()->estimate_);
    }

    return maxTaskEstimateTime;
}

RoundRobinNB::~RoundRobinNB() {
    for (const auto& entry : tasks_) {
        for (Task* task : entry) {
            delete task;
        }
    }
}

} // namespace task_manager

#include "RoundRobinNB.h"

#include "../Macro.h"


namespace task_manager {

void RoundRobinNB::NewTasks(unsigned nTask,
                            BoundedTimeT estimate, ResourceT cpuRequest, ResourceT memoryRequest, JobIdT jobID) {
    ASSERT(nTask != 0);
    ASSERT(static_cast<uint64_t>(taskCount_) + nTask < static_cast<uint64_t>(UINT32_MAX));

    taskCount_ += nTask;
    tasks_.push_back(std::move(std::vector<Task*>(nTask)));

    for (unsigned i = 0; i < nTask; ++i) {
        tasks_.back()[i] = Task::New(estimate, cpuRequest, memoryRequest, jobID);
        sumTaskEstimateTime_ += estimate;
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
    currentSize_ = taskCount_;
}

bool RoundRobinNB::IsThereSomethingElse() {
    return currentIter_ < currentSize_;
}

unsigned RoundRobinNB::TaskCount() {
    return taskCount_;
}

void RoundRobinNB::Sort() {
    it_ = tasks_.begin();

    currentIter_ = 0;
    currentSize_ = taskCount_;
}

uint64_t RoundRobinNB::SumTaskEstimateTime() {
    return sumTaskEstimateTime_;
}

BoundedTimeT RoundRobinNB::MaxTaskEstimateTime() {
    ASSERT(!tasks_.empty());

    BoundedTimeT maxTaskEstimateTime{0};
    for (const auto& entry : tasks_) {
        maxTaskEstimateTime = std::max(maxTaskEstimateTime, entry.front()->estimate_);
    }

    return maxTaskEstimateTime;
}

RoundRobinNB::~RoundRobinNB() {
    for (const auto& entry : tasks_) {
        for (Task* task : entry) {
            Task::Delete(task);
        }
    }
}

} // namespace task_manager

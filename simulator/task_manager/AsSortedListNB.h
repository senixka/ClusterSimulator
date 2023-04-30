#pragma once

#include "ITaskManager.h"
#include "../Macro.h"

#include <list>


namespace task_manager::detail {

template<class ComparePolicy>
class AsSortedListNB : public ITaskManager {
public:
    void PutTask(Task* task) override {
        tasks_.push_back(task);
        sumTaskEstimateTime_ += task->estimate_;
    }

    Task* GetTask() override {
        ASSERT(it_ != tasks_.end());
        return *it_;
    }

    void ReturnTask(Task* task, bool isScheduled) override {
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

    void NewSchedulingCycle() override {
        it_ = tasks_.begin();
    }

    size_t TaskCount() override {
        return tasks_.size();
    }

    void Sort() override {
        tasks_.sort(ComparePolicy::Compare);
        it_ = tasks_.begin();
    }

    unsigned __int128 SumTaskEstimateTime() override {
        return sumTaskEstimateTime_;
    }

    uint64_t MaxTaskEstimateTime() override {
        ASSERT(!tasks_.empty());

        uint64_t maxTaskEstimateTime = tasks_.front()->estimate_;
        for (Task* task : tasks_) {
            if (maxTaskEstimateTime < task->estimate_) {
                maxTaskEstimateTime = task->estimate_;
            }
        }

        return maxTaskEstimateTime;
    }

    ~AsSortedListNB() {
        for (Task* task : tasks_) {
            delete task;
        }
    }

private:
    std::list<Task*> tasks_;
    typename std::list<Task*>::iterator it_;
    unsigned __int128 sumTaskEstimateTime_{0};
};

} // namespace task_manager::detail
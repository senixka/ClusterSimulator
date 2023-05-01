#pragma once

#include "ITaskManager.h"
#include "../Macro.h"

#include <list>
#include <vector>


namespace task_manager::detail {


template<class ComparePolicy>
class AsSortedListNB : public ITaskManager {
public:
    void NewTasks(size_t kTask,
                  uint64_t estimate, unsigned cpuRequest, unsigned memoryRequest, unsigned jobID) override {
        ASSERT(kTask != 0);

        taskCount_ += kTask;
        tasks_.push_back(std::move(std::vector<Task*>(kTask)));

        for (size_t i = 0; i < kTask; ++i) {
            Task* task = new Task(estimate, cpuRequest, memoryRequest, jobID);

            tasks_.back()[i] = task;
            sumTaskEstimateTime_ += task->estimate_;
        }
    }

    Task* GetTask() override {
        ASSERT(it_ != tasks_.end() && !it_->empty());
        return it_->back();
    }

    void ReturnTask(Task* task, bool isScheduled) override {
        if (isScheduled) {
            it_->pop_back();
            sumTaskEstimateTime_ -= task->estimate_;
            --taskCount_;

            if (it_->empty()) {
                it_ = tasks_.erase(it_);
            }
        } else {
            ++it_;
        }
    }

    void NewSchedulingCycle() override {
        it_ = tasks_.begin();
    }

    bool IsThereSomethingElse() override {
        return it_ != tasks_.end();
    }

    size_t TaskCount() override {
        return taskCount_;
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

        uint64_t maxTaskEstimateTime{0};
        for (const auto& entry : tasks_) {
            maxTaskEstimateTime = std::max(maxTaskEstimateTime, entry.front()->estimate_);
        }

        return maxTaskEstimateTime;
    }

    ~AsSortedListNB() {
        for (const auto& entry : tasks_) {
            for (Task* task : entry) {
                delete task;
            }
        }
    }

private:
    std::list<std::vector<Task*>> tasks_;
    typename decltype(tasks_)::iterator it_;

    size_t taskCount_{0};
    unsigned __int128 sumTaskEstimateTime_{0};
};

} // namespace task_manager::detail
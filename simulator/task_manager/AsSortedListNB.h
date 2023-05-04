#pragma once

#include "ITaskManager.h"
#include "../Defines.h"
#include "../Macro.h"
#include "../Task.h"

#include <list>
#include <vector>


namespace task_manager::detail {


template<class ComparePolicy>
class AsSortedListNB : public ITaskManager {
public:
    void NewTasks(unsigned nTask,
                  BoundedTimeT estimate, ResourceT cpuRequest, ResourceT memoryRequest, JobIdT jobID) override {
        ASSERT(nTask != 0);
        ASSERT(static_cast<uint64_t>(taskCount_) + nTask < static_cast<uint64_t>(UINT32_MAX));

        taskCount_ += nTask;
        tasks_.push_back(std::move(std::vector<Task*>(nTask)));

        for (unsigned i = 0; i < nTask; ++i) {
            tasks_.back()[i] = Task::New(estimate, cpuRequest, memoryRequest, jobID);
            sumTaskEstimateTime_ += estimate;
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

    unsigned TaskCount() override {
        return taskCount_;
    }

    void Sort() override {
        tasks_.sort(ComparePolicy::Compare);
        it_ = tasks_.begin();
    }

    uint64_t SumTaskEstimateTime() override {
        return sumTaskEstimateTime_;
    }

    BoundedTimeT MaxTaskEstimateTime() override {
        ASSERT(!tasks_.empty());

        BoundedTimeT maxTaskEstimateTime{0};
        for (const auto& entry : tasks_) {
            maxTaskEstimateTime = std::max(maxTaskEstimateTime, entry.front()->estimate_);
        }

        return maxTaskEstimateTime;
    }

    ~AsSortedListNB() {
        for (const auto& entry : tasks_) {
            for (Task* task : entry) {
                Task::Delete(task);
            }
        }
    }

private:
    std::list<std::vector<Task*>> tasks_;
    typename decltype(tasks_)::iterator it_;

    unsigned taskCount_{0};
    uint64_t sumTaskEstimateTime_{0};
};

} // namespace task_manager::detail
#pragma once

#include "ITaskManager.h"
#include "../Macro.h"
#include "../BoundedTime.h"

#include <list>


namespace task_manager::detail {

template<class ComparePolicy>
class AsSortedListB : public ITaskManager {
public:
    void PutTask(Task* task) override {
        tasks_.push_back(task);

        sumTaskEstimateTime_ = BoundedSum(sumTaskEstimateTime_, task->estimate_);
    }

    Task* GetTask() override {
        ASSERT(!tasks_.empty());

        Task* task = tasks_.front();
        tasks_.pop_front();

        sumTaskEstimateTime_ -= task->estimate_;
        return task;
    }

    void ReturnTask(Task* task) override {
        tasks_.push_front(task);

        sumTaskEstimateTime_ = BoundedSum(sumTaskEstimateTime_, task->estimate_);
    }

    size_t TaskCount() override {
        return tasks_.size();
    }

    void Sort() override {
        tasks_.sort(ComparePolicy::Compare);
    }

    uint64_t SumTaskEstimateTime() override {
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

    ~AsSortedListB() {
        for (Task* task : tasks_) {
            delete task;
        }
    }

private:
    std::list<Task*> tasks_;
    uint64_t sumTaskEstimateTime_{0};
};

} // namespace task_manager::detail
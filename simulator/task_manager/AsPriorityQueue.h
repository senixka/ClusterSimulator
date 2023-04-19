#pragma once

#include "ITaskManager.h"
#include "../Macro.h"

#include <vector>
#include <algorithm>


namespace task_manager::detail {

template<class PriorityQueuePtrCmp>
class AsPriorityQueue : public ITaskManager {
public:
    void PutTask(Task* task) override {
        tasks_.push_back(task);
        std::push_heap(tasks_.begin(), tasks_.end(), cmp_);
    }

    Task* GetTask() override {
        ASSERT(!tasks_.empty());

        std::pop_heap(tasks_.begin(), tasks_.end(), cmp_);
        Task* task = tasks_.back();
        tasks_.pop_back();
        return task;
    }

    void ReturnTask(Task* task) override {
        tasks_.push_back(task);
        std::push_heap(tasks_.begin(), tasks_.end(), cmp_);
    }

    size_t TaskCount() override {
        return tasks_.size();
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

    ~AsPriorityQueue() {
        for (Task* task : tasks_) {
            delete task;
        }
    }

private:
    PriorityQueuePtrCmp cmp_;
    std::vector<Task*> tasks_;
};

} // namespace task_manager::detail
#include "InJobOrder.h"

#include "../Macro.h"


void InJobOrder::PutTask(Task* task) {
    tasks.push_back(task);
}

Task* InJobOrder::GetTask() {
    ASSERT(!tasks.empty());

    Task* task = tasks.front();
    tasks.pop_front();
    return task;
}

void InJobOrder::ReturnTask(Task* task) {
    tasks.push_front(task);
}

size_t InJobOrder::TaskCount() {
    return tasks.size();
}

uint64_t InJobOrder::MaxTaskEstimateTime() {
    ASSERT(!tasks.empty());

    uint64_t maxTaskEstimateTime = tasks.front()->estimate;
    for (const auto& task : tasks) {
        if (maxTaskEstimateTime < task->estimate) {
            maxTaskEstimateTime = task->estimate;
        }
    }

    return maxTaskEstimateTime;
}

InJobOrder::~InJobOrder() {
    for (Task* task : tasks) {
        delete task;
    }
}

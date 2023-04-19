#pragma once

#include "AsPriorityQueue.h"
#include "../Task.h"


namespace task_manager::detail {

class MinTimeFirstPriorityQueuePtrCmp {
public:
    bool operator() (const Task* lhs, const Task* rhs) const {
        return lhs->estimate_ > rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MinTimeFirst = class detail::AsPriorityQueue<detail::MinTimeFirstPriorityQueuePtrCmp>;

} // namespace task_manager

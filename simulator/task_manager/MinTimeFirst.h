#pragma once

#include "AsPriorityQueue.h"
#include "../Task.h"


namespace task_manager::detail {

class MinTimeFirstSortedListPtrCmp {
public:
    bool operator() (const Task* lhs, const Task* rhs) const {
        return lhs->estimate_ < rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MinTimeFirst = class detail::AsPriorityQueue<detail::MinTimeFirstSortedListPtrCmp>;

} // namespace task_manager

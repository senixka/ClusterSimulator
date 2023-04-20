#pragma once

#include "AsPriorityQueue.h"
#include "../Task.h"


namespace task_manager::detail {

class MaxTimeFirstSortedListPtrCmp {
public:
    bool operator()(const Task *lhs, const Task *rhs) const {
        return lhs->estimate_ > rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MaxTimeFirst = class detail::AsPriorityQueue<detail::MaxTimeFirstSortedListPtrCmp>;

} // namespace task_manager

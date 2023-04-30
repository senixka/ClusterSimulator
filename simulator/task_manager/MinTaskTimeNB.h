#pragma once

#include "AsSortedListNB.h"
#include "../Task.h"


namespace task_manager::detail {

struct MinTaskTimePtrCmp {
    static bool Compare(const Task* lhs, const Task* rhs) {
        return lhs->estimate_ < rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MinTaskTimeNB = class detail::AsSortedListNB<detail::MinTaskTimePtrCmp>;

} // namespace task_manager

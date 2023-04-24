#pragma once

#include "AsSortedListB.h"
#include "../Task.h"


namespace task_manager::detail {

struct MaxTaskTimePtrCmp {
    static bool Compare(const Task* lhs, const Task* rhs) {
        return lhs->estimate_ > rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MaxTaskTimeB = class detail::AsSortedListB<detail::MaxTaskTimePtrCmp>;

} // namespace task_manager

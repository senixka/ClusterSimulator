#pragma once

#include "AsSortedList.h"
#include "../Task.h"


namespace task_manager::detail {

struct MinTimeFirstPtrCmp {
    static bool Compare(const Task* lhs, const Task* rhs) {
        return lhs->estimate_ < rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MinTimeFirst = class detail::AsSortedList<detail::MinTimeFirstPtrCmp>;

} // namespace task_manager

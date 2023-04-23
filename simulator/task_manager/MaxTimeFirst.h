#pragma once

#include "AsSortedList.h"
#include "../Task.h"


namespace task_manager::detail {

struct MaxTimeFirstPtrCmp {
    static bool Compare(const Task* lhs, const Task* rhs) {
        return lhs->estimate_ > rhs->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MaxTimeFirst = class detail::AsSortedList<detail::MaxTimeFirstPtrCmp>;

} // namespace task_manager

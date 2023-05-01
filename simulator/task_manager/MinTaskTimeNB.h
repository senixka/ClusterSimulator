#pragma once

#include "AsSortedListNB.h"
#include "../Task.h"


namespace task_manager::detail {

struct MinTaskTimePtrCmp {
    static bool Compare (const std::vector<Task*>& lhs, const std::vector<Task*>& rhs) {
        return lhs.front()->estimate_ < rhs.front()->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MinTaskTimeNB = class detail::AsSortedListNB<detail::MinTaskTimePtrCmp>;

} // namespace task_manager

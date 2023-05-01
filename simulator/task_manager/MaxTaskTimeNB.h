#pragma once

#include "AsSortedListNB.h"
#include "../Task.h"


namespace task_manager::detail {

struct MaxTaskTimePtrCmp {
    static bool Compare (const std::vector<Task*>& lhs, const std::vector<Task*>& rhs) {
        return lhs.front()->estimate_ > rhs.front()->estimate_;
    }
};

} // namespace task_manager::detail


namespace task_manager {

using MaxTaskTimeNB = class detail::AsSortedListNB<detail::MaxTaskTimePtrCmp>;

} // namespace task_manager

#pragma once

#include "AsSortedListNB.h"
#include "../Job.h"


namespace job_manager::detail {

struct MinTaskCountPtrCmp {
    static bool Compare(const Job* lhs, const Job* rhs) {
        return lhs->taskManager_->TaskCount() < rhs->taskManager_->TaskCount();
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MinTaskCountNB = class detail::AsSortedListNB<detail::MinTaskCountPtrCmp>;

} // namespace job_manager

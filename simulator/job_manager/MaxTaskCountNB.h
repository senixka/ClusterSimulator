#pragma once

#include "AsSortedListNB.h"
#include "../Job.h"


namespace job_manager::detail {

struct MaxTaskCountPtrCmp {
    static bool Compare(const Job* lhs, const Job* rhs) {
        return lhs->taskManager_->TaskCount() > rhs->taskManager_->TaskCount();
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MaxTaskCountNB = class detail::AsSortedListNB<detail::MaxTaskCountPtrCmp>;

} // namespace job_manager

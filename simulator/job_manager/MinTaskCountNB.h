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

using MinTaskCountNB_1 = class detail::AsSortedListNB<detail::MinTaskCountPtrCmp, 1u>;
using MinTaskCountNB_10 = class detail::AsSortedListNB<detail::MinTaskCountPtrCmp, 10u>;
using MinTaskCountNB_100 = class detail::AsSortedListNB<detail::MinTaskCountPtrCmp, 100u>;

} // namespace job_manager

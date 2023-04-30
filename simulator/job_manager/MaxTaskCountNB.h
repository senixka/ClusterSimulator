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

using MaxTaskCountNB_1 = class detail::AsSortedListNB<detail::MaxTaskCountPtrCmp, 1u>;
using MaxTaskCountNB_10 = class detail::AsSortedListNB<detail::MaxTaskCountPtrCmp, 10u>;
using MaxTaskCountNB_100 = class detail::AsSortedListNB<detail::MaxTaskCountPtrCmp, 100u>;

} // namespace job_manager

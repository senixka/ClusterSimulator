#pragma once

#include "AsSortedListNB.h"
#include "../Job.h"


namespace job_manager::detail {

struct MaxSumTaskEstimatePtrCmp {
    static bool Compare(const Job* lhs, const Job* rhs) {
        return lhs->taskManager_->SumTaskEstimateTime() > rhs->taskManager_->SumTaskEstimateTime();
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MaxSumTaskEstimateNB_1 = class detail::AsSortedListNB<detail::MaxSumTaskEstimatePtrCmp, 1u>;
using MaxSumTaskEstimateNB_10 = class detail::AsSortedListNB<detail::MaxSumTaskEstimatePtrCmp, 10u>;
using MaxSumTaskEstimateNB_100 = class detail::AsSortedListNB<detail::MaxSumTaskEstimatePtrCmp, 100u>;

} // namespace job_manager

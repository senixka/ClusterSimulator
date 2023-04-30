#pragma once

#include "AsSortedListNB.h"
#include "../Job.h"


namespace job_manager::detail {

struct MinSumTaskEstimatePtrCmp {
    static bool Compare(const Job* lhs, const Job* rhs) {
        return lhs->taskManager_->SumTaskEstimateTime() < rhs->taskManager_->SumTaskEstimateTime();
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MinSumTaskEstimateNB_1 = class detail::AsSortedListNB<detail::MinSumTaskEstimatePtrCmp, 1u>;
using MinSumTaskEstimateNB_10 = class detail::AsSortedListNB<detail::MinSumTaskEstimatePtrCmp, 10u>;
using MinSumTaskEstimateNB_100 = class detail::AsSortedListNB<detail::MinSumTaskEstimatePtrCmp, 100u>;

} // namespace job_manager

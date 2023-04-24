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

using MaxSumTaskEstimateNB = class detail::AsSortedListNB<detail::MaxSumTaskEstimatePtrCmp>;

} // namespace job_manager

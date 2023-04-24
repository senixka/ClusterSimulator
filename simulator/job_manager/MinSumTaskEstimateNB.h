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

using MinSumTaskEstimateNB = class detail::AsSortedListNB<detail::MinSumTaskEstimatePtrCmp>;

} // namespace job_manager

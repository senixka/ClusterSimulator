#pragma once

#include "AsSortedList.h"
#include "../Job.h"


namespace job_manager::detail {

class MinSumTaskEstimateFirstNonBlockingSetPtrCmp {
public:
    bool operator() (const Job* lhs, const Job* rhs) const {
        return lhs->taskManager_->SumTaskEstimateTime() < rhs->taskManager_->SumTaskEstimateTime();
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MinSumTaskEstimateFirstNonBlocking = class detail::AsSortedList<detail::MinSumTaskEstimateFirstNonBlockingSetPtrCmp>;

} // namespace job_manager

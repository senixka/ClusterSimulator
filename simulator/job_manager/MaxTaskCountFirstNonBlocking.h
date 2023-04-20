#pragma once

#include "AsSortedList.h"
#include "../Job.h"


namespace job_manager::detail {

class MaxTaskCountFirstNonBlockingSetPtrCmp {
public:
    bool operator() (const Job* lhs, const Job* rhs) const {
        return lhs->taskManager_->TaskCount() > rhs->taskManager_->TaskCount();
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MaxTaskCountFirstNonBlocking = class detail::AsSortedList<detail::MaxTaskCountFirstNonBlockingSetPtrCmp>;

} // namespace job_manager

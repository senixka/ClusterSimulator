#pragma once

#include "AsSortedList.h"
#include "../Job.h"


namespace job_manager::detail {

class MaxJobSubmitTimeFirstNonBlockingListPtrCmp {
public:
    bool operator() (const Job* lhs, const Job* rhs) const {
        return lhs->jobTime_ > rhs->jobTime_;
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MaxJobSubmitTimeFirstNonBlocking = class detail::AsSortedList<detail::MaxJobSubmitTimeFirstNonBlockingListPtrCmp>;

} // namespace job_manager

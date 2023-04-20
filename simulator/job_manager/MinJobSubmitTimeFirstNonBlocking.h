#pragma once

#include "AsSortedList.h"
#include "../Job.h"


namespace job_manager::detail {

class MinJobSubmitTimeFirstNonBlockingSetPtrCmp {
public:
    bool operator() (const Job* lhs, const Job* rhs) const {
        return lhs->jobTime_ < rhs->jobTime_;
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MinJobSubmitTimeFirstNonBlocking = class detail::AsSortedList<detail::MinJobSubmitTimeFirstNonBlockingSetPtrCmp>;

} // namespace job_manager

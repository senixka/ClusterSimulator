#pragma once

#include "AsSortedListNB.h"
#include "../Job.h"


namespace job_manager::detail {

struct MaxJobSubmitTimePtrCmp {
    static bool Compare(const Job* lhs, const Job* rhs) {
        return lhs->jobTime_ > rhs->jobTime_;
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MaxJobSubmitTimeNB = class detail::AsSortedListNB<detail::MaxJobSubmitTimePtrCmp>;

} // namespace job_manager

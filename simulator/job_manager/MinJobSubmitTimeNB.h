#pragma once

#include "AsSortedListNB.h"
#include "../Job.h"


namespace job_manager::detail {

struct MinJobSubmitTimePtrCmp {
    static bool Compare(const Job* lhs, const Job* rhs) {
        return lhs->jobTime_ < rhs->jobTime_;
    }
};

} // namespace job_manager::detail


namespace job_manager {

using MinJobSubmitTimeNB_1 = class detail::AsSortedListNB<detail::MinJobSubmitTimePtrCmp, 1u>;
using MinJobSubmitTimeNB_10 = class detail::AsSortedListNB<detail::MinJobSubmitTimePtrCmp, 10u>;
using MinJobSubmitTimeNB_100 = class detail::AsSortedListNB<detail::MinJobSubmitTimePtrCmp, 100u>;

} // namespace job_manager

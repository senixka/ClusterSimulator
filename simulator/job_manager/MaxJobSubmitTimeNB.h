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

using MaxJobSubmitTimeNB_1 = class detail::AsSortedListNB<detail::MaxJobSubmitTimePtrCmp, 1u>;
using MaxJobSubmitTimeNB_10 = class detail::AsSortedListNB<detail::MaxJobSubmitTimePtrCmp, 10u>;
using MaxJobSubmitTimeNB_100 = class detail::AsSortedListNB<detail::MaxJobSubmitTimePtrCmp, 100u>;

} // namespace job_manager

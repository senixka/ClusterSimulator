#pragma once

#include "Defines.h"

#include <cstdint>
#include <tuple>


enum class ClusterEventType {
    TASK_FINISHED = 0,
    JOB_SUBMITTED = 1,
    RUN_SCHEDULER = 2,
    UPDATE_STATISTICS = 3,
};


class ClusterEvent {
public:
    ClusterEvent() = default;
    explicit ClusterEvent(BoundedTimeT eventTime, ClusterEventType clusterEventType)
        : eventTime_(eventTime), clusterEventType_(clusterEventType) {
    }

    virtual ~ClusterEvent() = default;

public:
    BoundedTimeT eventTime_{0};
    ClusterEventType clusterEventType_{ClusterEventType::RUN_SCHEDULER};
};


struct ClusterEventPtrCompare {
    bool operator() (const ClusterEvent* lhs, const ClusterEvent* rhs) const {
        return (lhs->eventTime_ > rhs->eventTime_) ||
               (lhs->eventTime_ == rhs->eventTime_ && lhs->clusterEventType_ > rhs->clusterEventType_);
    }
};

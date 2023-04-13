#pragma once

#include <cstdint>
#include <tuple>


enum ClusterEventType {
    TASK_FINISHED = 0,
    JOB_SUBMITTED = 1,
    RUN_SCHEDULER = 2,
    UPDATE_STATISTICS = 3,
};


class ClusterEvent {
public:
    ClusterEvent() = default;
    explicit ClusterEvent(uint64_t eventTime, ClusterEventType clusterEventType)
        : eventTime_(eventTime), clusterEventType_(clusterEventType) {
    }

    virtual ~ClusterEvent() = default;

public:
    uint64_t eventTime_{0};
    ClusterEventType clusterEventType_{ClusterEventType::RUN_SCHEDULER};
};


class ClusterEventPtrCompare {
public:
    bool operator() (const ClusterEvent* lhs, const ClusterEvent* rhs) const {
        return std::tie(lhs->eventTime_, lhs->clusterEventType_) > std::tie(rhs->eventTime_, rhs->clusterEventType_);
    }
};

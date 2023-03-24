#ifndef CLUSTERSIMULATOR_CLUSTEREVENT_H
#define CLUSTERSIMULATOR_CLUSTEREVENT_H

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
    uint64_t eventTime{0};
    ClusterEventType clusterEventType{ClusterEventType::RUN_SCHEDULER};

    ClusterEvent() = default;
    ClusterEvent(uint64_t eTime, ClusterEventType eType) : eventTime(eTime), clusterEventType(eType) {
    }

    virtual ~ClusterEvent() = default;
};

class ClusterEventPtrCompare {
public:
    bool operator() (const ClusterEvent* lhs, const ClusterEvent* rhs) {
        return std::tie(lhs->eventTime, lhs->clusterEventType) > std::tie(rhs->eventTime, rhs->clusterEventType);
    }
};


#endif //CLUSTERSIMULATOR_CLUSTEREVENT_H

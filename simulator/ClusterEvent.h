#ifndef CLUSTERSIMULATOR_CLUSTEREVENT_H
#define CLUSTERSIMULATOR_CLUSTEREVENT_H

#include <cstdint>
#include <tuple>


enum ClusterEventType {
    TASK_FINISHED,
    JOB_SUBMITTED,
    RUN_SCHEDULER,
    UPDATE_STATISTICS,
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
        return std::tie(lhs->eventTime, lhs->eventTime) > std::tie(rhs->eventTime, rhs->eventTime);
    }
};


#endif //CLUSTERSIMULATOR_CLUSTEREVENT_H

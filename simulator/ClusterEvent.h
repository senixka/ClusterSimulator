#ifndef CLUSTERSIMULATOR_CLUSTEREVENT_H
#define CLUSTERSIMULATOR_CLUSTEREVENT_H

#include <memory>


enum ClusterEventType {
    TASK_FINISHED,
    JOB_SUBMITTED,
    RUN_SCHEDULER,
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
    bool operator() (const std::shared_ptr<ClusterEvent>& lhs, const std::shared_ptr<ClusterEvent>& rhs) {
        return std::tie(lhs->eventTime, lhs->eventTime) > std::tie(rhs->eventTime, rhs->eventTime);
    }
};


#endif //CLUSTERSIMULATOR_CLUSTEREVENT_H

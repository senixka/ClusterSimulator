#ifndef CLUSTERSIMULATOR_SCHEDULERRANDOM_H
#define CLUSTERSIMULATOR_SCHEDULERRANDOM_H

#include "Scheduler.h"


class SchedulerRandom : public Scheduler {
public:
    void OnJobSubmitted(Cluster& cluster, Job* job) override;
    void OnTaskFinished(Cluster& cluster, Task* task) override;
    void Schedule(Cluster& cluster) override;
};


#endif //CLUSTERSIMULATOR_SCHEDULERRANDOM_H

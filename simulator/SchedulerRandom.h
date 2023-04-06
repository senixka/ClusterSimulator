#ifndef CLUSTERSIMULATOR_SCHEDULERRANDOM_H
#define CLUSTERSIMULATOR_SCHEDULERRANDOM_H

#include "Scheduler.h"


class SchedulerRandom : public Scheduler {
public:
    void OnJobSubmitted(const Job&) override;
    void OnTaskFinished(const Task&) override;
    void Schedule(Cluster& cluster) override;
};


#endif //CLUSTERSIMULATOR_SCHEDULERRANDOM_H

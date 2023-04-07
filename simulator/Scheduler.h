#ifndef CLUSTERSIMULATOR_SCHEDULER_H
#define CLUSTERSIMULATOR_SCHEDULER_H


#include "Task.h"
#include "Job.h"

class Cluster;


class Scheduler {
public:
    virtual void OnJobSubmitted(Cluster& cluster, Job* job) = 0;
    virtual void OnTaskFinished(Cluster&, Task* task) = 0;
    virtual void Schedule(Cluster& cluster) = 0;

    virtual ~Scheduler() = default;
};


#endif //CLUSTERSIMULATOR_SCHEDULER_H

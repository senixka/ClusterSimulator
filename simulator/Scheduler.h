#ifndef CLUSTERSIMULATOR_SCHEDULER_H
#define CLUSTERSIMULATOR_SCHEDULER_H


#include "Task.h"
#include "Job.h"

class Cluster;


class Scheduler {
public:
    void OnJobSubmitted(const Job&);

    void OnTaskFinished(const Task&);

    void Schedule(Cluster& cluster);
};


#endif //CLUSTERSIMULATOR_SCHEDULER_H

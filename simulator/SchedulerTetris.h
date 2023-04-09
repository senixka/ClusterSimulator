#ifndef CLUSTERSIMULATOR_SCHEDULERTETRIS_H
#define CLUSTERSIMULATOR_SCHEDULERTETRIS_H

#include "Task.h"
#include "Scheduler.h"
#include "MachineManager.h"


class SchedulerTetris : public Scheduler {
public:
    void OnJobSubmitted(Cluster& cluster, Job* job) override;
    void OnTaskFinished(Cluster& cluster, Task* task) override;
    void Schedule(Cluster& cluster) override;

private:
    unsigned BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task& task);

private:
    bool anyTaskFinished{true};
};


#endif //CLUSTERSIMULATOR_SCHEDULERTETRIS_H

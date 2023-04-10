#ifndef CLUSTERSIMULATOR_SCHEDULERVOLUME_H
#define CLUSTERSIMULATOR_SCHEDULERVOLUME_H

#include "Task.h"
#include "Scheduler.h"
#include "MachineManager.h"


class SchedulerMinVolume : public Scheduler {
public:
    void OnJobSubmitted(Cluster& cluster, Job* job) override;
    void OnTaskFinished(Cluster& cluster, Task* task) override;
    void Schedule(Cluster& cluster) override;

private:
    unsigned BestMachineIndex(const MachineManager::ReturnQueryType& machines) const;

private:
    bool anyTaskFinished{true};
};


#endif //CLUSTERSIMULATOR_SCHEDULERVOLUME_H

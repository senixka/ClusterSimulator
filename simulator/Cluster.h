#ifndef CLUSTERSIMULATOR_CLUSTER_H
#define CLUSTERSIMULATOR_CLUSTER_H

#include "Task.h"
#include "Job.h"
#include "Machine.h"
#include "Statistics.h"
#include "BoundedTime.h"
#include "MachineManager.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <list>

class Scheduler;
class SchedulerRandom;


class Cluster {
    ////////////////////// Cluster section /////////////////////////

    using EventQueueType = std::priority_queue<ClusterEvent*, std::vector<ClusterEvent*>, ClusterEventPtrCompare>;

    EventQueueType clusterEvents;
    EventQueueType deferEvents;
    std::list<Job*> currentJobs;

    uint64_t time{0};

    ////////////////////// Machine section /////////////////////////

    MachineManager* machineManager{nullptr};

    ////////////////////// Scheduler section ///////////////////////

    friend class SchedulerRandom;

    Scheduler* scheduler{nullptr};
    const uint64_t scheduleEachTime{10_S2MICROS};

    ////////////////////// Statistics section //////////////////////

    Statistics* statistics{nullptr};
    const uint64_t updateStatisticsEachTime{500_S2MICROS};

    long double currentUsedCPU{0};
    long double currentUsedMemory{0};
    long double currentUsedDisk{0};

    ////////////////////////////////////////////////////////////////

public:
    Cluster(const std::string& inputFilePath, MachineManager* machineManagerPtr, Scheduler* schedulerPtr, Statistics* statisticsPtr);
    ~Cluster();

    void Run();
    bool Update();

    void PutEvent(ClusterEvent*);

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task);

    void DeleteFinishedJobs();
};


#endif //CLUSTERSIMULATOR_CLUSTER_H

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


class Cluster {
public:
    ////////////////////// Cluster section /////////////////////////

    using EventQueueType = std::priority_queue<ClusterEvent*, std::vector<ClusterEvent*>, ClusterEventPtrCompare>;

    EventQueueType clusterEvents;
    EventQueueType deferEvents;
    std::list<Job*> currentJobs;

    uint64_t time{0};

    ////////////////////// Machine section /////////////////////////

    MachineManager* machineManager{nullptr};

    ////////////////////// Scheduler section ///////////////////////

    Scheduler* scheduler{nullptr};
    const uint64_t scheduleEachTime{10_S2MICROS};

    ////////////////////// Statistics section //////////////////////

    Statistics* statistics{nullptr};
    const uint64_t updateStatisticsEachTime{500_S2MICROS};

    ////////////////////////////////////////////////////////////////

public:
    Cluster(const std::string& inputFilePath, MachineManager* machineManagerPtr, Scheduler* schedulerPtr, Statistics* statisticsPtr);
    ~Cluster();

    void Run();
    bool Update();

    void PutEvent(ClusterEvent*);
    void PlaceTask(const Task& task);
    void DeleteFinishedJobs();
};


#endif //CLUSTERSIMULATOR_CLUSTER_H

#ifndef CLUSTERSIMULATOR_CLUSTER_H
#define CLUSTERSIMULATOR_CLUSTER_H

#include "Task.h"
#include "Job.h"
#include "Machine.h"
#include "Scheduler.h"
#include "Statistics.h"

#include <cstdlib>
#include <vector>
#include <queue>
#include <list>


class Cluster {
    uint64_t time = 0;

    const uint64_t scheduleEachTime = 1'000'000;
    std::vector<Machine> machines;

    std::list<Job*> currentJobs;
    std::priority_queue<ClusterEvent*, std::vector<ClusterEvent*>, ClusterEventPtrCompare> clusterEvents;

    Statistics statistics;

    friend class Scheduler;
    Scheduler scheduler;

    ////////////////////// Statistics section //////////////////////

    const uint64_t updateStatisticsEachTime{10'000'000'000};

    long double currentUsedCPU{0};
    long double currentUsedMemory{0};
    long double currentUsedDisk{0};

    ////////////////////////////////////////////////////////////////

public:

    Cluster(size_t machineN = 12500);

    void Run();
    bool Update();

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task);

    void DeleteFinishedJobs();

    void InitializeMachinesFromFile();
};


#endif //CLUSTERSIMULATOR_CLUSTER_H

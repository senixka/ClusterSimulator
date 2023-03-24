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
    using EventQueueType = std::priority_queue<ClusterEvent*, std::vector<ClusterEvent*>, ClusterEventPtrCompare>;

    uint64_t time = 0;

    const uint64_t scheduleEachTime = 10'000'000;
    std::vector<Machine> machines;

    std::list<Job*> currentJobs;
    EventQueueType clusterEvents;
    EventQueueType deferEvents;

    Statistics statistics;

    friend class Scheduler;
    Scheduler scheduler;

    ////////////////////// Statistics section //////////////////////

    const uint64_t updateStatisticsEachTime{1'000'000'000ull};

    long double currentUsedCPU{0};
    long double currentUsedMemory{0};
    long double currentUsedDisk{0};

    ////////////////////////////////////////////////////////////////

public:

    Cluster();
    ~Cluster();

    void Run();
    bool Update();

    uint64_t IncTime(uint64_t current_time, uint64_t shift);
    void PutEvent(ClusterEvent*);

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task);

    void DeleteFinishedJobs();

    void InitializeMachinesFromFile();
};


#endif //CLUSTERSIMULATOR_CLUSTER_H

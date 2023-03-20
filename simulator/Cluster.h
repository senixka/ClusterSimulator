#ifndef CLUSTERSIMULATOR_CLUSTER_H
#define CLUSTERSIMULATOR_CLUSTER_H

#include "Task.h"
#include "Job.h"
#include "Machine.h"
#include "Scheduler.h"
#include <cstdlib>
#include <vector>
#include <memory>
#include <queue>
#include <list>


class Cluster {
    friend class Scheduler;

    uint64_t time = 0;
    const uint64_t scheduleEachTime = 1'000'000;

    std::vector<Machine> machine;
    std::list<std::shared_ptr<Job>> currentJobs;
    std::priority_queue<
        std::shared_ptr<ClusterEvent>,
        std::vector<std::shared_ptr<ClusterEvent>>,
        ClusterEventPtrCompare
    > clusterEvents;

    long double usedCPU = 0;
    long double usedMemory = 0;
    long double usedDisk = 0;

    long double totalCPU = 0;
    long double totalMemory = 0;
    long double totalDisk = 0;

    uint64_t pendingTaskCount = 0;

    const uint64_t updateStatEach = 10'000'000;
    uint64_t updateStatTime = 0;

    std::vector<float> statCPU;
    std::vector<float> statMemory;
    std::vector<float> statDisk;
    std::vector<uint64_t> statTime;
    std::vector<uint64_t> statPendingTask;

    Scheduler scheduler;

public:

    Cluster(size_t machineN = 12500);

    void Run();
    bool Update();

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task);

    void DeleteFinishedJobs();

    void PrintClusterStat();
    void DumpStat();
};


#endif //CLUSTERSIMULATOR_CLUSTER_H

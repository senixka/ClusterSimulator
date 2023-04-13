#pragma once

#include "Task.h"
#include "Job.h"
#include "Machine.h"
#include "Statistics.h"
#include "BoundedTime.h"
#include "MachineManager.h"
#include "job_manager/IJobManager.h"
#include "task_manager/FactoryTaskManager.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <list>

class IScheduler;


class Cluster {
public:
    ////////////////////// Cluster section /////////////////////////

    using EventQueueType = std::priority_queue<ClusterEvent*, std::vector<ClusterEvent*>, ClusterEventPtrCompare>;

    uint64_t time{0};
    EventQueueType clusterEvents;

    ////////////////////// Machine section /////////////////////////

    MachineManager* machineManager{nullptr};

    ////////////////////// Job section /////////////////////////////

    IJobManager* jobManager{nullptr};

    ////////////////////// Scheduler section ///////////////////////

    IScheduler* scheduler{nullptr};
    const uint64_t scheduleEachTime{1_S2MICROS};

    ////////////////////// Statistics section //////////////////////

    Statistics* statistics{nullptr};
    const uint64_t updateStatisticsEachTime{10_S2MICROS};

    ////////////////////////////////////////////////////////////////

public:
    Cluster(const std::string& inputFilePath, MachineManager* machineManagerPtr, TaskManagerType taskManagerType,
            IJobManager* jobManagerPtr, IScheduler* schedulerPtr, Statistics* statisticsPtr);
    ~Cluster();

    void Run();
    bool Update();

    void PutEvent(ClusterEvent*);
    void PlaceTask(const Task& task) const;
};

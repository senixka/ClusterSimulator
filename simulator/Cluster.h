#pragma once

#include "Task.h"
#include "Job.h"
#include "Machine.h"
#include "Statistics.h"
#include "BoundedTime.h"
#include "MachineManager.h"
#include "job_manager/IJobManager.h"
#include "task_manager/FactoryTaskManager.h"
#include "job_manager/FactoryJobManager.h"

#include <cstdlib>
#include <vector>
#include <memory>
#include <string>
#include <queue>
#include <list>

class IScheduler;


class Cluster {
public:
    Cluster(const std::string& inputFilePath, TaskManagerType taskManagerType, std::shared_ptr<IJobManager> jobManager,
            std::shared_ptr<IScheduler> scheduler, std::shared_ptr<MachineManager> machineManager, std::shared_ptr<Statistics> statistics);
    ~Cluster();

    void Run();
    void PutEvent(ClusterEvent*);
    void PlaceTask(const Task& task) const;

private:
    bool Update();

public:
    using EventQueueType = std::priority_queue<ClusterEvent*, std::vector<ClusterEvent*>, ClusterEventPtrCompare>;

    uint64_t time_{0};
    EventQueueType clusterEvents_;

    std::shared_ptr<IJobManager> jobManager_{nullptr};

    std::shared_ptr<IScheduler> scheduler_{nullptr};
    const uint64_t scheduleEachTime_{500'000};

    std::shared_ptr<MachineManager> machineManager_{nullptr};

    std::shared_ptr<Statistics> statistics_{nullptr};
    const uint64_t updateStatisticsEachTime_{5_S2MICROS};
};

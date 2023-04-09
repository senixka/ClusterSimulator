#include "SchedulerRandom.h"

#include "MachineManager.h"
#include "Cluster.h"
#include "BoundedTime.h"

#include <random>

void SchedulerRandom::OnJobSubmitted(Cluster& cluster, Job* job) {
    MachineManager::ReturnQueryType machines;

    for (auto it = job->pendingTask.begin(); it != job->pendingTask.end();) {
        Task* task = *it;

        machines.clear();
        cluster.machineManager->FindSuitableMachines(*task, machines);

        if (machines.empty()) {
            ++it;
        } else {
            task->machineIndex = machines[std::rand() % machines.size()].second;
            task->eventTime = BoundedSum(cluster.time, task->estimate);
            task->clusterEventType = ClusterEventType::TASK_FINISHED;

            cluster.PlaceTask(*task);
            cluster.PutEvent(task);

            it = job->pendingTask.erase(it);
        }
    }
}

void SchedulerRandom::OnTaskFinished(Cluster& /*cluster*/, Task* /*task*/) {
    anyTaskFinished = true;
}

void SchedulerRandom::Schedule(Cluster& cluster) {
    if (!anyTaskFinished) {
        return;
    }

    MachineManager::ReturnQueryType machines;

    for (auto& job : cluster.currentJobs) {
        for (auto it = job->pendingTask.begin(); it != job->pendingTask.end();) {
            Task* task = *it;

            machines.clear();
            cluster.machineManager->FindSuitableMachines(*task, machines);

            if (machines.empty()) {
                ++it;
            } else {
                task->machineIndex = machines[std::rand() % machines.size()].second;
                task->eventTime = BoundedSum(cluster.time, task->estimate);
                task->clusterEventType = ClusterEventType::TASK_FINISHED;

                cluster.PlaceTask(*task);
                cluster.PutEvent(task);

                it = job->pendingTask.erase(it);
            }
        }
    }

    anyTaskFinished = false;
}
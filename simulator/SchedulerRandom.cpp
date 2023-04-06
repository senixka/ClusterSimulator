#include "SchedulerRandom.h"

#include "MachineManager.h"
#include "Cluster.h"

#include <random>


void SchedulerRandom::OnJobSubmitted(const Job&) {
}

void SchedulerRandom::OnTaskFinished(const Task&) {
}

void SchedulerRandom::Schedule(Cluster& cluster) {
    MachineManager::ReturnQueryType machines;

    for (auto& job : cluster.currentJobs) {
        for (auto it = job->pendingTask.begin(); it != job->pendingTask.end();) {
            Task* task = *it;
            task->machineIndex = UINT32_MAX;

            machines.clear();
            cluster.machineManager.FindSuitableMachines(*task, machines);

            if (machines.empty()) {
                ++it;
            } else {
                size_t machineIndex = machines[std::rand() % machines.size()].second;

                task->machineIndex = machineIndex;
                task->eventTime = cluster.IncTime(cluster.time, task->estimate);
                task->clusterEventType = ClusterEventType::TASK_FINISHED;

                cluster.PlaceTaskOnMachine(*task, machineIndex);
                cluster.PutEvent(task);

                it = job->pendingTask.erase(it);
            }
        }
    }
}
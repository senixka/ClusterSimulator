#include "SchedulerMinVolume.h"

#include "Cluster.h"
#include "BoundedTime.h"


unsigned SchedulerMinVolume::BestMachineIndex(const MachineManager::ReturnQueryType& machines) const {
    unsigned bestMachineIndex{0};
    double minVolume{10};

    for (const auto& [machineResources, machineIndex] : machines) {
        const auto mVolume = machineResources.get<0>() * machineResources.get<1>() * machineResources.get<2>();

        if (mVolume < minVolume) {
            minVolume = mVolume;
            bestMachineIndex = machineIndex;
        }
    }

    return bestMachineIndex;
}

void SchedulerMinVolume::OnJobSubmitted(Cluster& cluster, Job* job) {
    MachineManager::ReturnQueryType machines;

    for (auto it = job->pendingTask.begin(); it != job->pendingTask.end();) {
        Task* task = *it;

        machines.clear();
        cluster.machineManager->FindSuitableMachines(*task, machines);

        if (machines.empty()) {
            ++it;
        } else {
            task->machineIndex = BestMachineIndex(machines);
            task->eventTime = BoundedSum(cluster.time, task->estimate);
            task->clusterEventType = ClusterEventType::TASK_FINISHED;

            cluster.PlaceTask(*task);
            cluster.PutEvent(task);

            it = job->pendingTask.erase(it);
        }
    }
}

void SchedulerMinVolume::OnTaskFinished(Cluster& /*cluster*/, Task* /*task*/) {
    anyTaskFinished = true;
}

void SchedulerMinVolume::Schedule(Cluster& cluster) {
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
                task->machineIndex = BestMachineIndex(machines);
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
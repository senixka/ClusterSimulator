#include "SchedulerImpl.h"

#include "Cluster.h"
#include "BoundedTime.h"
#include "MachineManager.h"

#include <unordered_set>


SchedulerImpl::SchedulerImpl(IPlacingStrategy* currentPlacingStrategy)
    : placingStrategy(currentPlacingStrategy) {
}

void SchedulerImpl::OnJobSubmitted(Cluster& /*cluster*/) {
}

void SchedulerImpl::OnTaskFinished(Cluster& /*cluster*/) {
}

void SchedulerImpl::Schedule(Cluster& cluster) {
    MachineManager::ReturnQueryType machines;
    std::unordered_set<Job*> noModification;

    while (cluster.jobManager->JobCount() != 0) {
        Job* job = cluster.jobManager->GetJob();
        Task* task = job->taskManager->GetTask();

        machines.clear();
        cluster.machineManager->FindSuitableMachines(*task, machines);

        if (machines.empty()) {
            job->taskManager->ReturnTask(task);
            cluster.jobManager->ReturnJob(job, false);

            if (noModification.find(job) == noModification.end()) {
                noModification.insert(job);
            } else {
                break;
            }
        } else {
            task->machineIndex = placingStrategy->BestMachineIndex(machines, task);
            task->eventTime = BoundedSum(cluster.time, task->estimate);
            task->clusterEventType = ClusterEventType::TASK_FINISHED;

            cluster.PlaceTask(*task);
            cluster.PutEvent(task);

            cluster.jobManager->ReturnJob(job, true);

            noModification.erase(job);
        }
    }
}

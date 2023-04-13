#include "SchedulerImpl.h"

#include "Cluster.h"
#include "BoundedTime.h"
#include "MachineManager.h"

#include <unordered_set>


SchedulerImpl::SchedulerImpl(std::shared_ptr<IPlacingStrategy> placingStrategy)
    : placingStrategy_(placingStrategy) {
}

void SchedulerImpl::OnJobSubmitted(Cluster& /*cluster*/) {
}

void SchedulerImpl::OnTaskFinished(Cluster& /*cluster*/) {
}

void SchedulerImpl::Schedule(Cluster& cluster) {
    MachineManager::ReturnQueryType machines;
    std::unordered_set<Job*> noModification;

    while (cluster.jobManager_->JobCount() != 0) {
        Job* job = cluster.jobManager_->GetJob();
        Task* task = job->taskManager_->GetTask();

        machines.clear();
        cluster.machineManager_->FindSuitableMachines(*task, machines);

        if (machines.empty()) {
            job->taskManager_->ReturnTask(task);
            cluster.jobManager_->ReturnJob(job, false);

            if (noModification.find(job) == noModification.end()) {
                noModification.insert(job);
            } else {
                break;
            }
        } else {
            task->machineIndex_ = placingStrategy_->BestMachineIndex(machines, task);
            task->eventTime_ = BoundedSum(cluster.time_, task->estimate_);
            task->clusterEventType_ = ClusterEventType::TASK_FINISHED;

            cluster.PlaceTask(*task);
            cluster.PutEvent(task);

            cluster.jobManager_->ReturnJob(job, true);
            noModification.erase(job);
        }
    }
}

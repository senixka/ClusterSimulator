#include "SchedulerImpl.h"

#include "Cluster.h"
#include "BoundedTime.h"
#include "MachineManager.h"

#include <unordered_set>


SchedulerImpl::SchedulerImpl(std::shared_ptr<IPlacingStrategy> placingStrategy)
    : placingStrategy_(placingStrategy) {
}

void SchedulerImpl::OnJobSubmitted(Cluster& cluster) {
    Schedule(cluster);
}

void SchedulerImpl::Schedule(Cluster& cluster) {
    std::vector<const Machine*> machines;

    cluster.jobManager_->NewSchedulingCycle();

    while (cluster.jobManager_->IsThereSomethingElse()) {
        Job *job = cluster.jobManager_->GetJob();
        Task *task = job->taskManager_->GetTask();

        cluster.machineManager_->FindSuitableMachines(*task, machines);

        if (machines.empty()) {
            job->taskManager_->ReturnTask(task, false);
            cluster.jobManager_->ReturnJob(job, false);
        } else {
            task->machineIndex_ = placingStrategy_->BestMachineIndex(machines, task);
            task->eventTime_ = BoundedSum(cluster.time_, task->estimate_);
            task->clusterEventType_ = ClusterEventType::TASK_FINISHED;

            job->taskManager_->ReturnTask(task, true);
            cluster.jobManager_->ReturnJob(job, true);

            cluster.PlaceTask(*task);
            cluster.PutEvent(task);
        }
    }
}

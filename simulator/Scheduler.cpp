#include "Scheduler.h"

#include "Cluster.h"


void Scheduler::OnJobSubmitted(const Job&) {
}

void Scheduler::OnTaskFinished(const Task&) {
}

void Scheduler::Schedule(Cluster& cluster) {
//    //Place all task on first machine, ignore resources
//    for (auto& job : cluster.currentJobs) {
//        for (auto task : job->pendingTask) {
//            task->machineIndex = UINT32_MAX;
//            task->eventTime = cluster.IncTime(cluster.time, task->estimate);
//            task->clusterEventType = ClusterEventType::TASK_FINISHED;
//
//            cluster.PlaceTaskOnMachine(*task, 0);
//            cluster.PutEvent(task);
//        }
//
//        job->pendingTask.clear();
//    }

    //Place all task on first machine, with respect to resources
    std::vector<std::pair<point_3d, size_t>> machines;

    for (auto& job : cluster.currentJobs) {
        for (auto it = job->pendingTask.begin(); it != job->pendingTask.end();) {
            Task* task = *it;
            task->machineIndex = UINT32_MAX;

            machines.clear();
            cluster.machineManager.FindSuitableMachines(*task, machines);

            if (machines.empty()) {
                ++it;
            } else {
                size_t machineIndex = machines[0].second;

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

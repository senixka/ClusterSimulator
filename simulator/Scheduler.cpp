#include "Scheduler.h"

#include "Cluster.h"
#include <memory>

void Scheduler::OnJobSubmitted(const Job&) {
}

void Scheduler::OnTaskFinished(const Task&) {
}

void Scheduler::Schedule(Cluster& cluster) {
    //Place all task on first machine, ignore resources
    for (auto& job : cluster.currentJobs) {
        for (auto task : job->pendingTask) {
            task->machineIndex = UINT32_MAX;
            task->eventTime = cluster.IncTime(cluster.time, task->estimate);
            task->clusterEventType = ClusterEventType::TASK_FINISHED;

            cluster.PlaceTaskOnMachine(*task, 0);
            cluster.PutEvent(task);
        }

        job->pendingTask.clear();
    }


//    for (auto& job : cluster.currentJobs) {
//        for (auto task = job->pendingTask.begin(); task != job->pendingTask.end();) {
//            Task& newTask = *task;
//            newTask.machineIndex = UINT32_MAX;
//
//            for (size_t i = 0; i < cluster.machine.size(); ++i) {
//                if (cluster.machine[i].IsTaskPlaceable(newTask)) {
//                    newTask.eventTime = newTask.estimate != UINT64_MAX ? cluster.time + newTask.estimate : UINT64_MAX;
//                    newTask.clusterEventType = ClusterEventType::TASK_FINISHED;
//
//                    cluster.PlaceTaskOnMachine(newTask, i);
//
//                    if (newTask.eventTime != UINT64_MAX) {
//                        printf("Place task %\n");
//                        cluster.clusterEvents.push(std::make_shared<Task>(newTask));
//                    }
//                    break;
//                }
//            }
//
//            if (newTask.machineIndex == UINT32_MAX) {
//                ++task;
//            } else {
//                task = job->pendingTask.erase(task);
//            }
//        }
//    }
}

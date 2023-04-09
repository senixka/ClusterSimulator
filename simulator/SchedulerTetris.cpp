#include "SchedulerTetris.h"

#include "Cluster.h"
#include "BoundedTime.h"

#include <cmath>


unsigned SchedulerTetris::BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task& task) {
    unsigned bestMachineIndex{0};
    double minAngle{10};

    for (const auto& [machine_resources, machineIndex] : machines) {
        const auto& m_cpu = machine_resources.get<0>();
        const auto& m_mem = machine_resources.get<1>();
        const auto& m_disk = machine_resources.get<2>();

        const auto& t_cpu = task.cpuRequest;
        const auto& t_mem = task.memoryRequest;
        const auto& t_disk = task.diskSpaceRequest;

        auto dot_product = m_cpu * t_cpu + m_mem * t_mem + m_disk * t_disk;
        auto m_sq_norm = m_cpu * m_cpu + m_mem * m_mem + m_disk * m_disk;
        auto t_sq_norm = t_cpu * t_cpu + t_mem * t_mem + t_disk * t_disk;

        auto angle = std::acos(dot_product / std::sqrt(m_sq_norm * t_sq_norm));

        if (angle < minAngle) {
            minAngle = angle;
            bestMachineIndex = machineIndex;
        }
    }

    return bestMachineIndex;
}

void SchedulerTetris::OnJobSubmitted(Cluster& cluster, Job* job) {
    MachineManager::ReturnQueryType machines;

    for (auto it = job->pendingTask.begin(); it != job->pendingTask.end();) {
        Task* task = *it;

        machines.clear();
        cluster.machineManager->FindSuitableMachines(*task, machines);

        if (machines.empty()) {
            ++it;
        } else {
            task->machineIndex = BestMachineIndex(machines, *task);
            task->eventTime = BoundedSum(cluster.time, task->estimate);
            task->clusterEventType = ClusterEventType::TASK_FINISHED;

            cluster.PlaceTask(*task);
            cluster.PutEvent(task);

            it = job->pendingTask.erase(it);
        }
    }
}

void SchedulerTetris::OnTaskFinished(Cluster& /*cluster*/, Task* /*task*/) {
    anyTaskFinished = true;
}

void SchedulerTetris::Schedule(Cluster& cluster) {
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
                task->machineIndex = BestMachineIndex(machines, *task);
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

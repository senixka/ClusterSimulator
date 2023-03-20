#include "Cluster.h"

#include <fstream>
#include <string>
#include <numeric>

#include "../matplotlibcpp.h"
namespace plt = matplotlibcpp;


#define PANIC(S) printf("PANIC: " S); abort();


Cluster::Cluster(size_t machineN) {
    machine.resize(machineN);

    totalCPU = machine[0].cpuCapacity * machineN;
    totalMemory = machine[0].memoryCapacity * machineN;
    totalDisk = machine[0].diskSpaceCapacity * machineN;

    {
        std::ifstream fin;
        fin.open("../../prepared.txt");

        size_t jobN;
        fin >> jobN;

        for (size_t i = 0; i < jobN / 15; ++i) {
            auto job = std::make_shared<Job>(fin);
            job->eventTime = job->jobTime;
            job->clusterEventType = JOB_SUBMITTED;

            clusterEvents.push(job);
        }

        fin.close();
    }

    clusterEvents.push(std::make_shared<ClusterEvent>(0, ClusterEventType::RUN_SCHEDULER));
}

void Cluster::Run() {
    while (Update()) {
    }

    DumpStat();
}

bool Cluster::Update() {
    if (currentJobs.empty() && clusterEvents.size() == 1) {
        return false;
    }

    auto event = clusterEvents.top();
    clusterEvents.pop();
    time = event->eventTime;

    if (event->clusterEventType == ClusterEventType::JOB_SUBMITTED) {
        std::shared_ptr<Job> job = std::dynamic_pointer_cast<Job>(event);

        currentJobs.push_front(job);
        scheduler.OnJobSubmitted(*currentJobs.front());
    } else if (event->clusterEventType == ClusterEventType::TASK_FINISHED) {
        std::shared_ptr<Task> task = std::dynamic_pointer_cast<Task>(event);

        RemoveTaskFromMachine(*task);
        scheduler.OnTaskFinished(*task);
    } else if (event->clusterEventType == ClusterEventType::RUN_SCHEDULER) {
        PrintClusterStat();
        DeleteFinishedJobs();
        scheduler.Schedule(*this);

        event->eventTime += scheduleEachTime;
        clusterEvents.push(event);
    } else {
        PANIC("BAD EVENT TYPE")
    }

    while (time > updateStatTime) {
        statCPU.push_back(usedCPU * 100 / totalCPU);
        statMemory.push_back(usedMemory * 100 / totalMemory);
        statDisk.push_back(usedDisk * 100 / totalDisk);
        statTime.push_back(updateStatTime);
        statPendingTask.push_back(pendingTaskCount);

        updateStatTime += updateStatEach;
    }

    return true;
}

void Cluster::PlaceTaskOnMachine(Task& task, size_t machineIndex) {
    task.machineIndex = machineIndex;
    machine[machineIndex].PlaceTask(task);

    usedCPU += task.cpuRequest;
    usedMemory += task.memoryRequest;
    usedDisk += task.diskSpaceRequest;
    --pendingTaskCount;
}

void Cluster::RemoveTaskFromMachine(const Task& task) {
    machine[task.machineIndex].RemoveTask(task);

    usedCPU -= task.cpuRequest;
    usedMemory -= task.memoryRequest;
    usedDisk -= task.diskSpaceRequest;
}

void Cluster::DeleteFinishedJobs() {
    for (auto it = currentJobs.begin(); it != currentJobs.end();) {
        if ((*it)->pendingTask.empty()) {
            it = currentJobs.erase(it);
        } else {
            ++it;
        }
    }
}

void Cluster::PrintClusterStat() {
    printf("CPU: %2.2f%%  Memory: %2.2f%%  Disk: %2.2f%%  ", float(usedCPU * 100 / totalCPU), float(usedMemory * 100 / totalMemory), float(usedDisk * 100 / totalDisk));
    printf("%zu\t%zu\n", currentJobs.size(), clusterEvents.size());
}

void Cluster::DumpStat() {
    float avgCPU = std::accumulate(statCPU.begin(), statCPU.end(), static_cast<long double>(0)) / statCPU.size();
    float avgMemory = std::accumulate(statMemory.begin(), statMemory.end(), static_cast<long double>(0)) / statMemory.size();
    float avgDisk = std::accumulate(statDisk.begin(), statDisk.end(), static_cast<long double>(0)) / statDisk.size();

    plt::figure_size(1200, 500);
    plt::title("Cluster resource utilization in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Resource (in percent)");
    plt::plot(statTime, statCPU, {{"label", "CPU"}});
    plt::plot(statTime, std::vector<uint64_t>(statTime.size(), avgCPU), {{"label", "CPU AVG at " + std::to_string(avgCPU)}});
    plt::plot(statTime, statMemory, {{"label", "Memory"}});
    plt::plot(statTime, std::vector<uint64_t>(statTime.size(), avgMemory), {{"label", "Memory AVG at " + std::to_string(avgMemory)}});
    plt::plot(statTime, statDisk, {{"label", "Disk"}});
    plt::plot(statTime, std::vector<uint64_t>(statTime.size(), avgDisk), {{"label", "Disk AVG at " + std::to_string(avgDisk)}});
    plt::plot(statTime, statPendingTask, {{"label", "Pending"}});
    plt::legend();
    //plt::savefig("./stat/1_cluster_utilization_speedup_job_task_" + to_string(SPEEDUP_JOB_TIME) + "_" + to_string(SPEEDUP_TASK_TIME) + ".png");
    plt::show();
}
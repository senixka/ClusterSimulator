#include "Cluster.h"

#include <fstream>
#include <string>

#define PANIC(S) printf("PANIC: " S); abort();


Cluster::Cluster(size_t machineN) {
    InitializeMachinesFromFile();

    {
        std::ifstream fin;
        fin.open("../../prepared.txt");

        size_t jobN;
        fin >> jobN;

        for (size_t i = 0; i < jobN / 25; ++i) {
            Job* job = new Job(fin);
            job->eventTime = job->jobTime;
            job->clusterEventType = JOB_SUBMITTED;

            clusterEvents.push(job);
        }

        fin.close();
    }

    clusterEvents.push(new ClusterEvent(0, ClusterEventType::RUN_SCHEDULER));
    clusterEvents.push(new ClusterEvent(0, ClusterEventType::UPDATE_STATISTICS));
}

void Cluster::InitializeMachinesFromFile() {
    std::ifstream in;
    in.open("../../prepared_machine.txt");

    size_t nMachine;
    in >> nMachine;

    machines.resize(nMachine);

    for (size_t i = 0; i < nMachine; ++i) {
        in >> machines[i].cpuCapacity >> machines[i].memoryCapacity;
        machines[i].diskSpaceCapacity = 1;

        statistics.OnMachineAdded(machines[i]);
    }
}

void Cluster::Run() {
    while (Update()) {
    }

    statistics.OnSimulationFinished(time);
    statistics.DumpStatistics();
}

bool Cluster::Update() {
    if (currentJobs.empty() && clusterEvents.size() == 2) {
        return false;
    }

    auto event = clusterEvents.top();
    clusterEvents.pop();
    time = event->eventTime;

    if (event->clusterEventType == ClusterEventType::JOB_SUBMITTED) {
        Job* job = reinterpret_cast<Job*>(event);

        statistics.OnJobSubmitted(time, job);
        currentJobs.push_front(job);
        scheduler.OnJobSubmitted(*currentJobs.front());
    } else if (event->clusterEventType == ClusterEventType::TASK_FINISHED) {
        Task* task = reinterpret_cast<Task*>(event);

        statistics.OnTaskFinished(time, task);
        RemoveTaskFromMachine(*task);
        scheduler.OnTaskFinished(*task);
        delete task;
    } else if (event->clusterEventType == ClusterEventType::RUN_SCHEDULER) {
        statistics.PrintStatistics();
        DeleteFinishedJobs();
        scheduler.Schedule(*this);

        event->eventTime += scheduleEachTime;
        clusterEvents.push(event);
    } else if (event->clusterEventType == ClusterEventType::UPDATE_STATISTICS) {
        statistics.UpdateUtilization(time, currentUsedCPU, currentUsedMemory, currentUsedDisk);

        event->eventTime += updateStatisticsEachTime;
        clusterEvents.push(event);
    } else {
        PANIC("BAD EVENT TYPE")
    }

    return true;
}

void Cluster::PlaceTaskOnMachine(Task& task, size_t machineIndex) {
    task.machineIndex = machineIndex;
    machines[machineIndex].PlaceTask(task);

    currentUsedCPU += task.cpuRequest;
    currentUsedMemory += task.memoryRequest;
    currentUsedDisk += task.diskSpaceRequest;
}

void Cluster::RemoveTaskFromMachine(const Task& task) {
    machines[task.machineIndex].RemoveTask(task);

    currentUsedCPU -= task.cpuRequest;
    currentUsedMemory -= task.memoryRequest;
    currentUsedDisk-= task.diskSpaceRequest;
}

void Cluster::DeleteFinishedJobs() {
    for (auto it = currentJobs.begin(); it != currentJobs.end();) {
        if ((*it)->pendingTask.empty()) {
            Job* job = *it;
            it = currentJobs.erase(it);
            delete job;
        } else {
            ++it;
        }
    }
}

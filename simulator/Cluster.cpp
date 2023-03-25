#include "Cluster.h"

#include <fstream>
#include <string>

#define PANIC(S) printf("PANIC: " S); abort()


Cluster::Cluster() {
    InitializeMachinesFromFile();

    {
        std::ifstream fin;
        fin.open("../input/job_and_task.txt");

        size_t nJob;
        fin >> nJob;

        // To test speed up only
        nJob /= 15;

        statistics.nJobInSimulation = nJob;

        for (size_t i = 0; i < nJob; ++i) {
            Job* job = new Job(fin);
            job->eventTime = job->jobTime;
            job->clusterEventType = JOB_SUBMITTED;

            statistics.nTaskInSimulation += job->pendingTask.size();
            clusterEvents.push(job);
        }

        fin.close();
    }

    clusterEvents.push(new ClusterEvent(0, ClusterEventType::RUN_SCHEDULER));
    clusterEvents.push(new ClusterEvent(0, ClusterEventType::UPDATE_STATISTICS));
}

Cluster::~Cluster() {
    while (!clusterEvents.empty()) {
        auto event = clusterEvents.top();
        clusterEvents.pop();
        delete event;
    }

    for (auto job : currentJobs) {
        delete job;
    }
}

void Cluster::InitializeMachinesFromFile() {
    std::ifstream in;
    in.open("../input/machine.txt");

    size_t nMachine;
    in >> nMachine;

    machines.resize(nMachine);

    for (size_t i = 0; i < nMachine; ++i) {
        in >> machines[i].cpuCapacity >> machines[i].memoryCapacity >> machines[i].diskSpaceCapacity;

        statistics.OnMachineAdded(machines[i]);
    }
}

uint64_t Cluster::IncTime(uint64_t current_time, uint64_t shift) {
    uint64_t res;
    if (__builtin_add_overflow(current_time, shift, &res)) [[unlikely]] {
        res = UINT64_MAX;
    }
    return res;
}

void Cluster::PutEvent(ClusterEvent* event) {
    if (event->eventTime != UINT64_MAX) {
        clusterEvents.push(event);
    } else {
        deferEvents.push(event);
    }
}

void Cluster::Run() {
    while (Update()) {
    }

    while (!deferEvents.empty()) {
        auto event = deferEvents.top();
        deferEvents.pop();

        if (event->clusterEventType == ClusterEventType::TASK_FINISHED) {
            Task* task = reinterpret_cast<Task*>(event);

            RemoveTaskFromMachine(*task);

            delete task;
        } else {
            PANIC("BAD EVENT TYPE");
        }
    }
    DeleteFinishedJobs();


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

        currentJobs.push_front(job);
        statistics.OnJobSubmitted(time, *job);
        scheduler.OnJobSubmitted(*currentJobs.front());
    } else if (event->clusterEventType == ClusterEventType::TASK_FINISHED) {
        Task* task = reinterpret_cast<Task*>(event);

        RemoveTaskFromMachine(*task);
        statistics.OnTaskFinished(time, *task);
        scheduler.OnTaskFinished(*task);

        delete task;
    } else if (event->clusterEventType == ClusterEventType::RUN_SCHEDULER) {
        DeleteFinishedJobs();
        scheduler.Schedule(*this);

        event->eventTime = IncTime(time, scheduleEachTime);
        clusterEvents.push(event);
    } else if (event->clusterEventType == ClusterEventType::UPDATE_STATISTICS) {
        statistics.UpdateUtilization(time, currentUsedCPU, currentUsedMemory, currentUsedDisk);
        statistics.PrintStatistics();

        event->eventTime = IncTime(time, updateStatisticsEachTime);
        clusterEvents.push(event);
    } else {
        PANIC("BAD EVENT TYPE");
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

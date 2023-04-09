#include "Cluster.h"

#include "SchedulerRandom.h"

#include <fstream>

#ifndef NDEBUG
#define PANIC(S) printf("PANIC: " S); abort()
#else
#define PANIC(S)
#endif


Cluster::Cluster(const std::string& inputFilePath, MachineManager* machineManagerPtr, Scheduler* schedulerPtr, Statistics* statisticsPtr)
    : machineManager(machineManagerPtr), scheduler(schedulerPtr), statistics(statisticsPtr) {
    for (const auto& machine : machineManager->GetAllMachines()) {
        statistics->OnMachineAdded(machine);
    }

    // Task and Job input
    {
        std::ifstream fin;
        fin.open(inputFilePath);

        size_t nJob;
        fin >> nJob;

        // To test speed up only
        nJob /= 15;

        statistics->nJobInSimulation = nJob;

        for (size_t i = 0; i < nJob; ++i) {
            Job* job = new Job(fin);
            job->eventTime = job->jobTime;
            job->clusterEventType = JOB_SUBMITTED;

            statistics->nTaskInSimulation += job->pendingTask.size();
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

void Cluster::PutEvent(ClusterEvent* event) {
    if (AtBound(event->eventTime)) [[unlikely]] {
        deferEvents.push(event);
    } else [[likely]] {
        clusterEvents.push(event);
    }
}

void Cluster::Run() {
    while (Update()) {
    }

    for (auto& job : currentJobs) {
        for (auto task : job->pendingTask) {
            delete task;
        }
        job->pendingTask.clear();
    }
    DeleteFinishedJobs();

    while (!deferEvents.empty()) {
        auto event = deferEvents.top();
        deferEvents.pop();

        if (event->clusterEventType == ClusterEventType::TASK_FINISHED) {
            Task* task = reinterpret_cast<Task*>(event);
            delete task;
        } else {
            PANIC("BAD EVENT TYPE");
        }
    }

    statistics->OnSimulationFinished(time);
}

bool Cluster::Update() {
    if (statistics->nJobInSimulation == statistics->jobSubmittedCounter) {
        return false;
    }

    auto event = clusterEvents.top();
    clusterEvents.pop();
    time = event->eventTime;

    if (event->clusterEventType == ClusterEventType::TASK_FINISHED) {
        Task* task = reinterpret_cast<Task*>(event);

        RemoveTaskFromMachine(*task);
        statistics->OnTaskFinished(time, *task);
        scheduler->OnTaskFinished(*this, task);

        delete task;
    } else if (event->clusterEventType == ClusterEventType::JOB_SUBMITTED) {
        Job* job = reinterpret_cast<Job*>(event);

        currentJobs.push_front(job);
        statistics->OnJobSubmitted(time, *job);
        scheduler->OnJobSubmitted(*this, job);
    } else if (event->clusterEventType == ClusterEventType::RUN_SCHEDULER) {
        DeleteFinishedJobs();
        scheduler->Schedule(*this);

        event->eventTime = BoundedSum(time, scheduleEachTime);
        clusterEvents.push(event);
    } else if (event->clusterEventType == ClusterEventType::UPDATE_STATISTICS) {
        statistics->UpdateUtilization(time, currentUsedCPU, currentUsedMemory, currentUsedDisk);
        statistics->PrintStatistics();

        event->eventTime = BoundedSum(time, updateStatisticsEachTime);
        clusterEvents.push(event);
    } else {
        PANIC("BAD EVENT TYPE");
    }

    return true;
}

void Cluster::PlaceTaskOnMachine(Task& task, size_t machineIndex) {
    task.machineIndex = machineIndex;
    machineManager->PlaceTaskOnMachine(task, machineIndex);

    currentUsedCPU += task.cpuRequest;
    currentUsedMemory += task.memoryRequest;
    currentUsedDisk += task.diskSpaceRequest;
}

void Cluster::RemoveTaskFromMachine(const Task& task) {
    machineManager->RemoveTaskFromMachine(task, task.machineIndex);

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

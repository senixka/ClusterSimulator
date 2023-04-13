#include "Cluster.h"

#include "IScheduler.h"

#include <fstream>

#ifndef NDEBUG
#define PANIC(S) printf("PANIC: " S); abort()
#else
#define PANIC(S)
#endif


Cluster::Cluster(const std::string& inputFilePath, MachineManager* machineManagerPtr, TaskManagerType taskManagerType,
                 IJobManager* jobManagerPtr, IScheduler* schedulerPtr, Statistics* statisticsPtr)
    : machineManager(machineManagerPtr), jobManager(jobManagerPtr), scheduler(schedulerPtr), statistics(statisticsPtr) {

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
        // Job /= 30;

        statistics->nJobInSimulation = nJob;

        for (size_t i = 0; i < nJob; ++i) {
            Job* job = new Job(taskManagerType, fin);
            job->eventTime = job->jobTime;
            job->clusterEventType = JOB_SUBMITTED;

            statistics->nTaskInSimulation += job->taskManager->TaskCount();
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
}

void Cluster::PutEvent(ClusterEvent* event) {
    if (AtBound(event->eventTime)) [[unlikely]] {
        assert(event->clusterEventType == ClusterEventType::TASK_FINISHED);

        Task* task = reinterpret_cast<Task*>(event);
        delete task;
    } else [[likely]] {
        clusterEvents.push(event);
    }
}

void Cluster::Run() {
    while (Update()) {
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

        machineManager->RemoveTaskFromMachine(*task);
        statistics->OnTaskFinished(time, *task);
        scheduler->OnTaskFinished(*this);

        delete task;
    } else if (event->clusterEventType == ClusterEventType::JOB_SUBMITTED) {
        Job* job = reinterpret_cast<Job*>(event);

        jobManager->PutJob(job);
        statistics->OnJobSubmitted(time, *job);
        scheduler->OnJobSubmitted(*this);
    } else if (event->clusterEventType == ClusterEventType::RUN_SCHEDULER) {
        scheduler->Schedule(*this);

        event->eventTime = BoundedSum(time, scheduleEachTime);
        clusterEvents.push(event);
    } else if (event->clusterEventType == ClusterEventType::UPDATE_STATISTICS) {
        statistics->UpdateStats(time);
        statistics->PrintStatistics();

        event->eventTime = BoundedSum(time, updateStatisticsEachTime);
        clusterEvents.push(event);
    } else {
        PANIC("BAD EVENT TYPE");
    }

    return true;
}

void Cluster::PlaceTask(const Task& task) const {
    machineManager->PlaceTaskOnMachine(task);
    statistics->OnTaskScheduled(time, task);
}

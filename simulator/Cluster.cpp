#include "Cluster.h"

#include "IScheduler.h"
#include "Macro.h"

#include <fstream>


Cluster::Cluster(const std::string& inputFilePath, TaskManagerType taskManagerType, std::shared_ptr<IJobManager> jobManager,
                 std::shared_ptr<IScheduler> scheduler, std::shared_ptr<MachineManager> machineManager, std::shared_ptr<Statistics> statistics)
    : jobManager_(jobManager), scheduler_(scheduler), machineManager_(machineManager), statistics_(statistics) {

    for (const auto& machine : machineManager_->GetAllMachines()) {
        statistics_->OnMachineAdded(machine);
    }

    // Task and Job input
    {
        std::ifstream fin;
        fin.open(inputFilePath);

        size_t nJob;
        fin >> nJob;

        // To test speed up only
        // nJob /= 30;

        statistics_->nJobInSimulation_ = nJob;

        for (size_t i = 0; i < nJob; ++i) {
            Job* job = new Job(taskManagerType, fin);
            job->eventTime_ = job->jobTime_;
            job->clusterEventType_ = ClusterEventType::JOB_SUBMITTED;

            statistics_->nTaskInSimulation_ += job->taskManager_->TaskCount();
            clusterEvents_.push(job);
        }

        fin.close();
    }

    clusterEvents_.push(new ClusterEvent(0, ClusterEventType::RUN_SCHEDULER));
    clusterEvents_.push(new ClusterEvent(0, ClusterEventType::UPDATE_STATISTICS));
}

Cluster::~Cluster() {
    while (!clusterEvents_.empty()) {
        auto event = clusterEvents_.top();
        clusterEvents_.pop();

        delete event;
    }
}

void Cluster::PutEvent(ClusterEvent* event) {
    if (AtBound(event->eventTime_)) [[unlikely]] {
        assert(event->clusterEventType_ == ClusterEventType::TASK_FINISHED);

        Task* task = reinterpret_cast<Task*>(event);
        delete task;
    } else [[likely]] {
        clusterEvents_.push(event);
    }
}

void Cluster::Run() {
    while (Update()) {
    }

    statistics_->OnSimulationFinished(time_);
}

bool Cluster::Update() {
    if (statistics_->nJobInSimulation_ == statistics_->jobSubmittedCounter_) {
        return false;
    }

    auto event = clusterEvents_.top();
    clusterEvents_.pop();
    time_ = event->eventTime_;

    if (event->clusterEventType_ == ClusterEventType::TASK_FINISHED) {
        Task* task = reinterpret_cast<Task*>(event);

        machineManager_->RemoveTaskFromMachine(*task);
        statistics_->OnTaskFinished(time_, *task);
        scheduler_->OnTaskFinished(*this);

        delete task;
    } else if (event->clusterEventType_ == ClusterEventType::JOB_SUBMITTED) {
        Job* job = reinterpret_cast<Job*>(event);

        jobManager_->PutJob(job);
        statistics_->OnJobSubmitted(time_, *job);
        scheduler_->OnJobSubmitted(*this);
    } else if (event->clusterEventType_ == ClusterEventType::RUN_SCHEDULER) {
        scheduler_->Schedule(*this);

        event->eventTime_ = BoundedSum(time_, scheduleEachTime_);
        clusterEvents_.push(event);
    } else if (event->clusterEventType_ == ClusterEventType::UPDATE_STATISTICS) {
        statistics_->UpdateStats(time_);
        statistics_->PrintStatistics();

        event->eventTime_ = BoundedSum(time_, updateStatisticsEachTime_);
        clusterEvents_.push(event);
    } else {
        PANIC("BAD EVENT TYPE");
    }

    return true;
}

void Cluster::PlaceTask(const Task& task) const {
    machineManager_->PlaceTaskOnMachine(task);
    statistics_->OnTaskScheduled(time_, task);
}

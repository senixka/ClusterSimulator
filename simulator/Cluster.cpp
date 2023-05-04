#include "Cluster.h"

#include "BoundedTime.h"
#include "IScheduler.h"
#include "Macro.h"

#include <fstream>


Cluster::Cluster(const std::string& inputFilePath, TaskManagerType taskManagerType, std::shared_ptr<IJobManager> jobManager,
                 std::shared_ptr<IScheduler> scheduler, std::shared_ptr<MachineManager> machineManager, std::shared_ptr<Statistics> statistics)
    : jobManager_(jobManager), scheduler_(scheduler), machineManager_(machineManager), statistics_(statistics) {
    printf("Start of Cluster construction\n");

    for (const auto& machine : machineManager_->GetAllMachines()) {
        statistics_->OnMachineAdded(machine);
    }

    std::ifstream fin;
    fin.open(inputFilePath);

    size_t nJob, nTask;
    fin >> nJob >> nTask;

    ASSERT(nJob < UINT32_MAX);
    ASSERT(nTask < UINT32_MAX);

    Job::PrepareMemoryPool(static_cast<unsigned>(nJob));
    Task::PrepareMemoryPool(static_cast<unsigned>(nTask));


    for (size_t i = 0; i < nJob; ++i) {
        Job* job = Job::New(taskManagerType, fin, static_cast<unsigned>(i + 1));
        job->eventTime_ = job->jobTime_;
        job->clusterEventType_ = ClusterEventType::JOB_SUBMITTED;

        ASSERT(!AtBound(job->eventTime_));

        clusterEvents_.push(job);
        statistics_->nTaskInSimulation_ += job->taskManager_->TaskCount();
        ++statistics->nJobInSimulation_;
    }

    fin.close();

    ASSERT(statistics_->nJobInSimulation_ == nJob);
    ASSERT(statistics_->nTaskInSimulation_ == nTask);

    clusterEvents_.push(new ClusterEvent(0u, ClusterEventType::RUN_SCHEDULER));
    clusterEvents_.push(new ClusterEvent(0u, ClusterEventType::UPDATE_STATISTICS));

    printf("End of Cluster construction\n");
}

void Cluster::DeleteEvent(ClusterEvent* event) {
    if (event->clusterEventType_ == ClusterEventType::TASK_FINISHED) {
        Task* task = dynamic_cast<Task*>(event);
        ASSERT(task);
        Task::Delete(task);
    } else if (event->clusterEventType_ == ClusterEventType::JOB_SUBMITTED) {
        Job* job = dynamic_cast<Job*>(event);
        ASSERT(job);
        Job::Delete(job);
    } else {
        delete event;
    }
}

Cluster::~Cluster() {
    while (!clusterEvents_.empty()) {
        auto event = clusterEvents_.top();
        clusterEvents_.pop();

        DeleteEvent(event);
    }
}

void Cluster::PutEvent(ClusterEvent* event) {
    if (AtBound(event->eventTime_)) [[unlikely]] {
        ASSERT(event->clusterEventType_ == ClusterEventType::TASK_FINISHED);
        DeleteEvent(event);
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
    if (statistics_->currentPendingTaskCounter_ > kCriticalPendingSize) [[unlikely]] {
        return false;
    }

    if (statistics_->nJobInSimulation_ == statistics_->jobSubmittedCounter_) [[unlikely]] {
        return false;
    }

    auto event = clusterEvents_.top();
    clusterEvents_.pop();
    time_ = event->eventTime_;

    if (event->clusterEventType_ == ClusterEventType::TASK_FINISHED) {
        Task* task = dynamic_cast<Task*>(event);
        ASSERT(task);

        machineManager_->RemoveTaskFromMachine(*task);
        statistics_->OnTaskFinished(time_, *task);

        Task::Delete(task);
    } else if (event->clusterEventType_ == ClusterEventType::JOB_SUBMITTED) {
        Job* job = dynamic_cast<Job*>(event);
        ASSERT(job);

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
    statistics_->OnTaskScheduled(task);
}

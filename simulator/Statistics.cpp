#include "Statistics.h"

#include "Macro.h"

#include <fstream>


Statistics::Statistics(const std::string& outputFilePath, const std::string& jobManagerName,
                       const std::string& taskManagerName, const std::string& placingStrategyName)
    : outputFilePath_(outputFilePath), jobManagerName_(jobManagerName),
      taskManagerName_(taskManagerName), placingStrategyName_(placingStrategyName) {
}

void Statistics::UpdateStats(BoundedTimeT currentTime) {
    utilizationCPUNumerator_.push_back(currentUsedCPU_);
    utilizationMemoryNumerator_.push_back(currentUsedMemory_);
    utilizationMeasurementsTime_.push_back(currentTime);

    pendingTask_.push_back(currentPendingTaskCounter_);
    workingTask_.push_back(currentWorkingTaskCounter_);
}

void Statistics::OnJobSubmitted(BoundedTimeT currentTime, const Job& job) {
    ++jobSubmittedCounter_;
    ++currentUnfinishedJobCounter_;

    currentPendingTaskCounter_ += job.taskManager_->TaskCount();
    maxPendingTask_ = std::max(maxPendingTask_, currentPendingTaskCounter_);

    jobStartTime_[job.jobId_] = currentTime;
    jobUnfinishedTaskCount_[job.jobId_] = job.taskManager_->TaskCount();
    jobIdealEstimateTime_[job.jobId_] = job.taskManager_->MaxTaskEstimateTime();
}

void Statistics::OnTaskScheduled(const Task& task) {
    ++currentWorkingTaskCounter_;
    --currentPendingTaskCounter_;

    currentUsedCPU_ += task.cpuRequest_;
    currentUsedMemory_ += task.memoryRequest_;
}

void Statistics::OnTaskFinished(BoundedTimeT currentTime, const Task& task) {
    --currentWorkingTaskCounter_;
    ++taskFinishedCounter_;

    currentUsedCPU_ -= task.cpuRequest_;
    currentUsedMemory_ -= task.memoryRequest_;

    ASSERT(jobUnfinishedTaskCount_.find(task.jobId_) != jobUnfinishedTaskCount_.end());

    if (--jobUnfinishedTaskCount_.at(task.jobId_) == 0) {
        jobUnfinishedTaskCount_.erase(task.jobId_);

        --currentUnfinishedJobCounter_;

        jobEndTime_[task.jobId_] = currentTime;
    }
}

void Statistics::OnSimulationFinished(BoundedTimeT currentTime) {
    makeSpanTime_ = currentTime;
}

void Statistics::OnMachineAdded(const Machine& machine) {
    totalAvailableCPU_ += machine.availableCpu_;
    totalAvailableMemory_ += machine.availableMemory_;
}

void Statistics::PrintStatistics() const {
    if (utilizationMeasurementsTime_.empty()) [[unlikely]] {
        printf("Stats: NO_INFO\n");
    } else [[likely]] {
        printf("CPU: %7.3Lf%%  Memory: %7.3Lf%%", static_cast<long double>(utilizationCPUNumerator_.back()) / totalAvailableCPU_ * 100,
                                                  static_cast<long double>(utilizationMemoryNumerator_.back()) / totalAvailableMemory_ * 100);
        printf("  Job submitted: %lu / %lu  Task finished: %lu / %lu", jobSubmittedCounter_, nJobInSimulation_, taskFinishedCounter_, nTaskInSimulation_);
        printf("  Task working: %lu  Task pending: %lu  Current Job: %lu\n", currentWorkingTaskCounter_, currentPendingTaskCounter_, currentUnfinishedJobCounter_);
    }
}

void Statistics::DumpStatistics() const {
    std::ofstream fout;
    fout.open(outputFilePath_);

    fout << "JobManagerName\n" << jobManagerName_ << std::endl;
    fout << "TaskManagerName\n" << taskManagerName_ << std::endl;
    fout << "PlacingStrategyName\n" << placingStrategyName_ << std::endl;

    fout << "nJobInSimulation\n" << nJobInSimulation_ << std::endl;
    fout << "nTaskInSimulation\n" << nTaskInSimulation_ << std::endl;

    fout << "TotalAvailableCPU\n" << totalAvailableCPU_ << std::endl;
    fout << "TotalAvailableMemory\n" << totalAvailableMemory_ << std::endl;

    fout << "MakeSpan\n" << makeSpanTime_ << std::endl;

    fout << "MaxPendingTask\n" << maxPendingTask_ << std::endl;

    fout << "PendingTaskCounter\n" << currentPendingTaskCounter_ << std::endl;
    fout << "UnfinishedJobCounter\n" << currentUnfinishedJobCounter_ << std::endl;
    fout << "TaskFinishedCounter\n" << taskFinishedCounter_ << std::endl;

    fout << "UtilizationMeasurementsTime UtilizationCPUNumerator UtilizationMemoryNumerator PendingTask WorkingTask" << std::endl;
    fout << utilizationMeasurementsTime_.size() << std::endl;

    for (size_t i = 0; i < utilizationMeasurementsTime_.size(); ++i) {
        fout << utilizationMeasurementsTime_[i] << " ";
        fout << utilizationCPUNumerator_[i] << " " << utilizationMemoryNumerator_[i] << " ";
        fout << pendingTask_[i] << " " << workingTask_[i] << std::endl;
    }

    fout << "JobStartTime JobEndTime JobIdealEstimateTime" << std::endl;
    fout << jobEndTime_.size() << std::endl;

    for (const auto &[jobID, endTime] : jobEndTime_) {
        fout << jobStartTime_.at(jobID) << " " << endTime << " " << jobIdealEstimateTime_.at(jobID) << std::endl;
    }

    fout.close();
}

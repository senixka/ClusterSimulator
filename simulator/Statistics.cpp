#include "Statistics.h"

#include "Macro.h"

#include <cmath>
#include <fstream>


Statistics::Statistics(const std::string& outputFilePath)
    : outputFilePath_(outputFilePath) {
}

void Statistics::UpdateStats(uint64_t currentTime) {
    utilizationMeasurementsTime_.push_back(currentTime);

    utilizationCPU_.push_back(static_cast<float>(currentUsedCPU_ * 100 / totalAvailableCPU_));
    utilizationMemory_.push_back(static_cast<float>(currentUsedMemory_ * 100 / totalAvailableMemory_));
    utilizationDisk_.push_back(static_cast<float>(currentUsedDisk_ * 100 / totalAvailableDisk_));

    pendingTask_.push_back(currentPendingTaskCounter_);
    workingTask_.push_back(currentWorkingTaskCounter_);
}

void Statistics::OnJobSubmitted(uint64_t currentTime, const Job& job) {
    ++jobSubmittedCounter_;
    ++currentUnfinishedJobCounter_;
    currentPendingTaskCounter_ += job.taskManager_->TaskCount();

    jobStartTime_[job.jobID_] = currentTime;
    jobUnfinishedTaskCount_[job.jobID_] = job.taskManager_->TaskCount();
    jobMinEstimateTime_[job.jobID_] = job.taskManager_->MaxTaskEstimateTime();
}

void Statistics::OnTaskScheduled(uint64_t /*currentTime*/, const Task& task) {
    ++currentWorkingTaskCounter_;
    --currentPendingTaskCounter_;

    currentUsedCPU_ += task.cpuRequest_;
    currentUsedMemory_ += task.memoryRequest_;
    currentUsedDisk_ += task.diskSpaceRequest_;
}

void Statistics::OnTaskFinished(uint64_t currentTime, const Task& task) {
    ++taskFinishedCounter_;
    --currentWorkingTaskCounter_;

    currentUsedCPU_ -= task.cpuRequest_;
    currentUsedMemory_ -= task.memoryRequest_;
    currentUsedDisk_ -= task.diskSpaceRequest_;

    if (--jobUnfinishedTaskCount_.at(task.jobID_) == 0) {
        jobEndTime_[task.jobID_] = currentTime;
        jobCompletionTime_.push_back(currentTime);
        --currentUnfinishedJobCounter_;
    }
}

void Statistics::OnSimulationFinished(uint64_t currentTime) {
    ////////////////////// MakeSpan /////////////////////////
    {
        makeSpanTime_ = currentTime;
    }

    ////////////////////// Average Utilization //////////////
    {
        long double sumCPU = 0, sumMemory = 0, sumDisk = 0;
        for (size_t i = 0; i < utilizationMeasurementsTime_.size(); ++i) {
            sumCPU += utilizationCPU_[i];
            sumMemory += utilizationMemory_[i];
            sumDisk += utilizationDisk_[i];
        }

        averageUtilizationCPU_ = static_cast<float>(sumCPU / utilizationMeasurementsTime_.size());
        averageUtilizationMemory_ = static_cast<float>(sumMemory / utilizationMeasurementsTime_.size());
        averageUtilizationDisk_ = static_cast<float>(sumDisk / utilizationMeasurementsTime_.size());
    }

    ////////////////////// Job's ANP ////////////////////////
    {
        for (const auto &[jobID, endTime] : jobEndTime_) {
            jobANP_[jobID] = static_cast<long double>(jobMinEstimateTime_.at(jobID)) / (endTime - jobStartTime_.at(jobID));
            minANP_ = std::min(minANP_, jobANP_[jobID]);
            maxANP_ = std::max(maxANP_, jobANP_[jobID]);
        }
    }

    ////////////////////// SNP //////////////////////////////
    {
        long double snp = 0;
        for (const auto &[jobID, anp] : jobANP_) {
            snp += std::log(anp);
        }
        snp = std::exp(snp / jobANP_.size());

        simulationSNP_ = snp;
    }

    ////////////////////// Unfairness ///////////////////////
    {
        long double meanANP = 0;
        for (const auto &[jobID, anp] : jobANP_) {
            meanANP += anp;
        }
        meanANP /= jobANP_.size();

        long double stdDeviation = 0;
        for (const auto &[jobID, anp] : jobANP_) {
            stdDeviation += (anp - meanANP) * (anp - meanANP);
        }
        stdDeviation = std::pow(stdDeviation / jobANP_.size(), 0.5L);

        simulationUnfairness_ = stdDeviation * 100 / meanANP;
    }

    ////////////////////// Slowdown 2-norm /////////////////////
    {
        long double slowdown = 0;
        for (const auto &[jobID, anp] : jobANP_) {
            slowdown += 1.0L / (anp * anp);
        }

        simulationSlowdown2Norm_ = std::sqrt(slowdown / jobANP_.size());
    }
}

void Statistics::OnMachineAdded(const Machine& machine) {
    totalAvailableCPU_ += machine.currentCpu_;
    totalAvailableMemory_ += machine.currentMemory_;
    totalAvailableDisk_ += machine.currentDiskSpace_;
}

void Statistics::PrintStatistics() const {
    if (utilizationMeasurementsTime_.empty()) [[unlikely]] {
        printf("CPU: NO INFO  Memory: NO INFO  Disk: NO INFO\n");
    } else [[likely]] {
        printf("CPU: %7.3f%%  Memory: %7.3f%%  Disk: %7.3f%%  ", utilizationCPU_.back(), utilizationMemory_.back(), utilizationDisk_.back());
        printf("Job submitted: %lu / %lu  Task finished: %lu / %lu", jobSubmittedCounter_, nJobInSimulation_, taskFinishedCounter_, nTaskInSimulation_);
        printf("  Task working: %lu  Task pending: %lu  Current Job: %lu\n", currentWorkingTaskCounter_, currentPendingTaskCounter_, currentUnfinishedJobCounter_);
    }
}

void Statistics::DumpStatistics() {
    ASSERT(minANP_ >= 0.0);
    ASSERT(maxANP_ <= 1.0);

    std::ofstream out;
    out.open(outputFilePath_);

    out << "MakeSpan\n" << makeSpanTime_ << std::endl;
    out << "PendingTaskCounter\n" << currentPendingTaskCounter_ << std::endl;
    out << FIXED_PRC << "SNP\n" << simulationSNP_ << std::endl;
    out << FIXED_PRC << "Unfairness\n" << simulationUnfairness_ << std::endl;
    out << FIXED_PRC << "Slowdown\n" << simulationSlowdown2Norm_ << std::endl;
    out << FIXED_PRC << "MinANP\n" << minANP_ << std::endl;
    out << FIXED_PRC << "MaxANP\n" << maxANP_ << std::endl;
    out << FIXED_PRC << "AverageUtilizationCPU\n" << averageUtilizationCPU_ << std::endl;
    out << FIXED_PRC << "AverageUtilizationMemory\n" << averageUtilizationMemory_ << std::endl;
    out << FIXED_PRC << "AverageUtilizationDisk\n" << averageUtilizationDisk_ << std::endl;
    out << FIXED_PRC << "TotalAvailableCPU\n" << totalAvailableCPU_ << std::endl;
    out << FIXED_PRC << "TotalAvailableMemory\n" << totalAvailableMemory_ << std::endl;
    out << FIXED_PRC << "TotalAvailableDiskSpace\n" << totalAvailableDisk_ << std::endl;


    out << "UtilizationMeasurementsTime UtilizationCPU UtilizationMemory UtilizationDisk PendingTask WorkingTask" << std::endl;
    out << utilizationMeasurementsTime_.size() << std::endl;

    for (size_t i = 0; i < utilizationMeasurementsTime_.size(); ++i) {
        out << utilizationMeasurementsTime_[i] << " ";
        out << FIXED_PRC << utilizationCPU_[i] << " " << utilizationMemory_[i] << " " << utilizationDisk_[i] << " ";
        out << pendingTask_[i] << " " << workingTask_[i] << std::endl;
    }

    out << "JobStartTime JobEndTime JobMinEstimateTime" << std::endl;
    out << jobEndTime_.size() << std::endl;

    for (const auto &[jobID, endTime] : jobEndTime_) {
        out << jobStartTime_.at(jobID) << " " << endTime << " " << jobMinEstimateTime_.at(jobID) << std::endl;
    }

    out.close();
}

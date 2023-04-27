#include "Statistics.h"

#include "Macro.h"

#include <fstream>


Statistics::Statistics(const std::string& outputFilePath) : outputFilePath_(outputFilePath) {
}

void Statistics::UpdateStats(uint64_t currentTime) {
    utilizationCPUNumerator_.push_back(currentUsedCPU_);
    utilizationMemoryNumerator_.push_back(currentUsedMemory_);
    utilizationMeasurementsTime_.push_back(currentTime);

    pendingTask_.push_back(currentPendingTaskCounter_);
    workingTask_.push_back(currentWorkingTaskCounter_);
}

void Statistics::OnJobSubmitted(uint64_t currentTime, const Job& job) {
    ++jobSubmittedCounter_;
    ++currentUnfinishedJobCounter_;

    currentPendingTaskCounter_ += job.taskManager_->TaskCount();

    jobStartTime_[job.jobID_] = currentTime;
    jobUnfinishedTaskCount_[job.jobID_] = job.taskManager_->TaskCount();
    jobIdealEstimateTime_[job.jobID_] = job.taskManager_->MaxTaskEstimateTime();
}

void Statistics::OnTaskScheduled(const Task& task) {
    ++currentWorkingTaskCounter_;
    --currentPendingTaskCounter_;

    currentUsedCPU_ += task.cpuRequest_;
    currentUsedMemory_ += task.memoryRequest_;
}

void Statistics::OnTaskFinished(uint64_t currentTime, const Task& task) {
    --currentWorkingTaskCounter_;
    ++taskFinishedCounter_;

    currentUsedCPU_ -= task.cpuRequest_;
    currentUsedMemory_ -= task.memoryRequest_;

    ASSERT(jobUnfinishedTaskCount_.find(task.jobID_) != jobUnfinishedTaskCount_.end());

    if (--jobUnfinishedTaskCount_.at(task.jobID_) == 0) {
        jobUnfinishedTaskCount_.erase(task.jobID_);

        --currentUnfinishedJobCounter_;

        jobEndTime_[task.jobID_] = currentTime;
    }
}

void Statistics::OnSimulationFinished(uint64_t currentTime) {
    ////////////////////// MakeSpan /////////////////////////
    {
        makeSpanTime_ = currentTime;
    }

    ////////////////////// Average Utilization //////////////
//    {
//        long double sumCPU = 0, sumMemory = 0, sumDisk = 0;
//        for (size_t i = 0; i < utilizationMeasurementsTime_.size(); ++i) {
//            sumCPU += utilizationCPU_[i];
//            sumMemory += utilizationMemory_[i];
//            sumDisk += utilizationDisk_[i];
//        }
//
//        averageUtilizationCPU_ = static_cast<float>(sumCPU / utilizationMeasurementsTime_.size());
//        averageUtilizationMemory_ = static_cast<float>(sumMemory / utilizationMeasurementsTime_.size());
//        averageUtilizationDisk_ = static_cast<float>(sumDisk / utilizationMeasurementsTime_.size());
//    }

    ////////////////////// Job's ANP ////////////////////////
//    {
//        for (const auto &[jobID, endTime] : jobEndTime_) {
//            jobANP_[jobID] = static_cast<long double>(jobMinEstimateTime_.at(jobID)) / (endTime - jobStartTime_.at(jobID));
//            minANP_ = std::min(minANP_, jobANP_[jobID]);
//            maxANP_ = std::max(maxANP_, jobANP_[jobID]);
//        }
//    }

    ////////////////////// SNP //////////////////////////////
//    {
//        long double snp = 0;
//        for (const auto &[jobID, anp] : jobANP_) {
//            snp += std::log(anp);
//        }
//        snp = std::exp(snp / jobANP_.size());
//
//        simulationSNP_ = snp;
//    }

    ////////////////////// Unfairness ///////////////////////
//    {
//        long double meanANP = 0;
//        for (const auto &[jobID, anp] : jobANP_) {
//            meanANP += anp;
//        }
//        meanANP /= jobANP_.size();
//
//        long double stdDeviation = 0;
//        for (const auto &[jobID, anp] : jobANP_) {
//            stdDeviation += (anp - meanANP) * (anp - meanANP);
//        }
//        stdDeviation = std::pow(stdDeviation / jobANP_.size(), 0.5L);
//
//        simulationUnfairness_ = stdDeviation * 100 / meanANP;
//    }

    ////////////////////// Slowdown 2-norm /////////////////////
//    {
//        long double slowdown = 0;
//        for (const auto &[jobID, anp] : jobANP_) {
//            slowdown += 1.0L / (anp * anp);
//        }
//
//        simulationSlowdown2Norm_ = std::sqrt(slowdown / jobANP_.size());
//    }
}

void Statistics::OnMachineAdded(const Machine& machine) {
    totalAvailableCPU_ += machine.availableCpu_;
    totalAvailableMemory_ += machine.availableMemory_;
}

void Statistics::PrintStatistics() const {
    if (utilizationMeasurementsTime_.empty()) [[unlikely]] {
        printf("CPU: NO INFO  Memory: NO INFO  Disk: NO INFO\n");
    } else [[likely]] {
        printf("CPU: %7.3Lf%%  Memory: %7.3Lf%%", static_cast<long double>(utilizationCPUNumerator_.back()) / totalAvailableCPU_ * 100,
                                                  static_cast<long double>(utilizationMemoryNumerator_.back()) / totalAvailableMemory_ * 100);
        printf("  Job submitted: %lu / %lu  Task finished: %lu / %lu", jobSubmittedCounter_, nJobInSimulation_, taskFinishedCounter_, nTaskInSimulation_);
        printf("  Task working: %lu  Task pending: %lu  Current Job: %lu\n", currentWorkingTaskCounter_, currentPendingTaskCounter_, currentUnfinishedJobCounter_);
    }
}

void Statistics::DumpStatistics() {
    std::ofstream fout;
    fout.open(outputFilePath_);

    fout << "MakeSpan\n" << makeSpanTime_ << std::endl;

    fout << "TotalAvailableCPU\n" << totalAvailableCPU_ << std::endl;
    fout << "TotalAvailableMemory\n" << totalAvailableMemory_ << std::endl;

    fout << "PendingTaskCounter\n" << currentPendingTaskCounter_ << std::endl;
    fout << "UnfinishedJobCounter\n" << currentUnfinishedJobCounter_ << std::endl;

    fout << "nJobInSimulation\n" << nJobInSimulation_ << std::endl;
    fout << "nTaskInSimulation\n" << nTaskInSimulation_ << std::endl;
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

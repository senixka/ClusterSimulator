#include "Statistics.h"

#include "BoundedTime.h"
#include "Macro.h"

#include <cmath>
#include <iomanip>
#include <fstream>

#include "../matplotlibcpp.h"
namespace plt = matplotlibcpp;


Statistics::Statistics(const std::string& simulationName)
    : simulationName_(simulationName) {
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

    if (--jobUnfinishedTaskCount_[task.jobID_] == 0) {
        jobEndTime_[task.jobID_] = currentTime;
        jobCompletionTime_.push_back(currentTime);
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
            jobANP_[jobID] = static_cast<long double>(jobMinEstimateTime_[jobID]) / (endTime - jobStartTime_[jobID]);
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
        printf("Job submitted: %lu / %lu \tTask finished: %lu / %lu", jobSubmittedCounter_, nJobInSimulation_, taskFinishedCounter_, nTaskInSimulation_);
        printf("\tTask working: %lu \tTask pending: %lu\n", currentWorkingTaskCounter_, currentPendingTaskCounter_);
    }
}

void Statistics::DumpStatistics() {
    ASSERT(minANP_ >= 0.0);
    ASSERT(maxANP_ <= 1.0);

    std::cout << std::fixed << std::setprecision(10) << "MakeSpan: " << makeSpanTime_ << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "PendingTaskCounter: " << currentPendingTaskCounter_ << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "SNP: " << simulationSNP_ << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Unfairness: " << simulationUnfairness_ << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Slowdown: " << simulationSlowdown2Norm_ << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Min ANP: " << minANP_ << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Max ANP: " << maxANP_ << std::endl;

    std::for_each(utilizationMeasurementsTime_.begin(), utilizationMeasurementsTime_.end(), [](uint64_t& value) { value /= MICROS_IN_S; });
    const auto& uTimes = utilizationMeasurementsTime_;

    ///////////////////////// Task Pending vs Working /////////////////////////

    plt::figure_size(1200, 500);
    plt::title("Task Pending vs Working in Time");
    plt::xlabel("Time (in seconds)");
    plt::ylabel("Count");

    plt::plot(uTimes, workingTask_, {{"label", "Working task"}});
    plt::plot(uTimes, pendingTask_, {{"label", "Pending task"}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask_[0]}, {{"label", "Slowdown: " + std::to_string(simulationSlowdown2Norm_)}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask_[0]}, {{"label", "Unfairness: " + std::to_string(simulationUnfairness_)}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask_[0]}, {{"label", "SNP: " + std::to_string(simulationSNP_)}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask_[0]}, {{"label", "Min ANP: " + std::to_string(minANP_)}});

    plt::legend();
    plt::savefig("../output/" + simulationName_ + "_cluster_pending_working.png");
    plt::show();

    //////////////////////////// Utilization //////////////////////////////

    plt::figure_size(1200, 500);
    plt::title("Cluster resource utilization in Time");
    plt::xlabel("Time (in seconds)");
    plt::ylabel("Resource (in percent)");

    plt::plot(uTimes, utilizationCPU_, {{"label", "CPU"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationCPU_), {{"label", "CPU AVG at " + std::to_string(averageUtilizationCPU_)}});

    plt::plot(uTimes, utilizationMemory_, {{"label", "Memory"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationMemory_), {{"label", "Memory AVG at " + std::to_string(averageUtilizationMemory_)}});

    plt::plot(uTimes, utilizationDisk_, {{"label", "Disk"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationDisk_), {{"label", "Disk AVG at " + std::to_string(averageUtilizationDisk_)}});

    plt::legend();
    plt::savefig("../output/" + simulationName_ + "_cluster_utilization.png");
    plt::show();
}

#include "Statistics.h"

#include "BoundedTime.h"

#include <cmath>
#include <iomanip>

#include "../matplotlibcpp.h"
namespace plt = matplotlibcpp;


Statistics::Statistics(const std::string& sName) : name(sName){
}

void Statistics::UpdateStats(uint64_t currentTime) {
    utilizationMeasurementsTime.push_back(currentTime);

    utilizationCPU.push_back(static_cast<float>(currentUsedCPU * 100 / totalAvailableCPU));
    utilizationMemory.push_back(static_cast<float>(currentUsedMemory * 100 / totalAvailableMemory));
    utilizationDisk.push_back(static_cast<float>(currentUsedDisk * 100 / totalAvailableDisk));

    pendingTask.push_back(currentPendingTaskCounter);
    workingTask.push_back(currentWorkingTaskCounter);
}

void Statistics::OnJobSubmitted(uint64_t currentTime, const Job& job) {
    ++jobSubmittedCounter;
    currentPendingTaskCounter += job.pendingTask.size();

    jobStartTime[job.jobID] = currentTime;
    jobUnfinishedTaskCount[job.jobID] = job.pendingTask.size();

    uint64_t maxTaskEstimateTime = job.pendingTask.front()->estimate;
    for (const auto& task : job.pendingTask) {
        if (maxTaskEstimateTime < task->estimate) {
            maxTaskEstimateTime = task->estimate;
        }
    }
    jobMinEstimateTime[job.jobID] = maxTaskEstimateTime;
}

void Statistics::OnTaskScheduled(uint64_t /*currentTime*/, const Task& task) {
    ++currentWorkingTaskCounter;
    --currentPendingTaskCounter;

    currentUsedCPU += task.cpuRequest;
    currentUsedMemory += task.memoryRequest;
    currentUsedDisk += task.diskSpaceRequest;
}

void Statistics::OnTaskFinished(uint64_t currentTime, const Task& task) {
    ++taskFinishedCounter;
    --currentWorkingTaskCounter;

    currentUsedCPU -= task.cpuRequest;
    currentUsedMemory -= task.memoryRequest;
    currentUsedDisk -= task.diskSpaceRequest;

    if (--jobUnfinishedTaskCount[task.jobID] == 0) {
        jobEndTime[task.jobID] = currentTime;
        jobCompletionTime.push_back(currentTime);
    }
}

void Statistics::OnSimulationFinished(uint64_t currentTime) {
    ////////////////////// MakeSpan /////////////////////////
    {
        makeSpanTime = currentTime;
    }

    ////////////////////// Average Utilization //////////////
    {
        long double sumCPU = 0, sumMemory = 0, sumDisk = 0;
        for (size_t i = 0; i < utilizationMeasurementsTime.size(); ++i) {
            sumCPU += utilizationCPU[i];
            sumMemory += utilizationMemory[i];
            sumDisk += utilizationDisk[i];
        }

        averageUtilizationCPU = static_cast<float>(sumCPU / utilizationMeasurementsTime.size());
        averageUtilizationMemory = static_cast<float>(sumMemory / utilizationMeasurementsTime.size());
        averageUtilizationDisk = static_cast<float>(sumDisk / utilizationMeasurementsTime.size());
    }

    ////////////////////// Job's ANP ////////////////////////
    {
        for (const auto &[jobID, endTime] : jobEndTime) {
            jobANP[jobID] = static_cast<long double>(jobMinEstimateTime[jobID]) / (endTime - jobStartTime[jobID]);
            minANP = std::min(minANP, jobANP[jobID]);
            maxANP = std::max(maxANP, jobANP[jobID]);
        }
    }

    ////////////////////// SNP //////////////////////////////
    {
        long double snp = 0;
        for (const auto &[jobID, anp] : jobANP) {
            snp += std::log(anp);
        }
        snp = std::exp(snp / jobANP.size());

        simulationSNP = snp;
    }

    ////////////////////// Unfairness ///////////////////////
    {
        long double meanANP = 0;
        for (const auto &[jobID, anp] : jobANP) {
            meanANP += anp;
        }
        meanANP /= jobANP.size();

        long double stdDeviation = 0;
        for (const auto &[jobID, anp] : jobANP) {
            stdDeviation += (anp - meanANP) * (anp - meanANP);
        }
        stdDeviation = std::pow(stdDeviation / jobANP.size(), 0.5L);

        simulationUnfairness = stdDeviation * 100 / meanANP;
    }

    ////////////////////// Slowdown 2-norm /////////////////////
    {
        long double slowdown = 0;
        for (const auto &[jobID, anp] : jobANP) {
            slowdown += 1.0L / (anp * anp);
        }

        simulationSlowdown2Norm = std::sqrt(slowdown / jobANP.size());
    }
}

void Statistics::OnMachineAdded(const Machine& machine) {
    totalAvailableCPU += machine.currentCpu;
    totalAvailableMemory += machine.currentMemory;
    totalAvailableDisk += machine.currentDiskSpace;
}

void Statistics::PrintStatistics() {
    if (utilizationMeasurementsTime.empty()) [[unlikely]] {
        printf("CPU: NO INFO  Memory: NO INFO  Disk: NO INFO\n");
    } else [[likely]] {
        printf("CPU: %7.3f%%  Memory: %7.3f%%  Disk: %7.3f%%  ", utilizationCPU.back(), utilizationMemory.back(), utilizationDisk.back());
        printf("Job submitted: %lu / %lu \tTask finished: %lu / %lu", jobSubmittedCounter, nJobInSimulation, taskFinishedCounter, nTaskInSimulation);
        printf("\tTask working: %lu \tTask pending: %lu\n", currentWorkingTaskCounter, currentPendingTaskCounter);
    }
}

void Statistics::DumpStatistics() {
    std::cout << std::fixed << std::setprecision(10) << "MakeSpan: " << makeSpanTime << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "PendingTaskCounter: " << currentPendingTaskCounter << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "SNP: " << simulationSNP << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Unfairness: " << simulationUnfairness << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Slowdown: " << simulationSlowdown2Norm << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Min ANP: " << minANP << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Max ANP: " << maxANP << std::endl;

    std::for_each(utilizationMeasurementsTime.begin(), utilizationMeasurementsTime.end(), [](uint64_t& value) { value /= MICROS_IN_S; });
    const auto& uTimes = utilizationMeasurementsTime;

    ///////////////////////// Task Pending vs Working /////////////////////////

    plt::figure_size(1200, 500);
    plt::title("Task Pending vs Working in Time");
    plt::xlabel("Time (in seconds)");
    plt::ylabel("Count");

    plt::plot(uTimes, workingTask, {{"label", "Working task"}});
    plt::plot(uTimes, pendingTask, {{"label", "Pending task"}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask[0]}, {{"label", "Slowdown: " + std::to_string(simulationSlowdown2Norm)}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask[0]}, {{"label", "Unfairness: " + std::to_string(simulationUnfairness)}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask[0]}, {{"label", "SNP: " + std::to_string(simulationSNP)}});
    plt::plot(std::vector<uint64_t>{uTimes[0]}, std::vector<uint64_t>{pendingTask[0]}, {{"label", "Min ANP: " + std::to_string(minANP)}});

    plt::legend();
    plt::savefig("../output/" + name + "_cluster_pending_working.png");
    plt::show();

    //////////////////////////// Utilization //////////////////////////////

    plt::figure_size(1200, 500);
    plt::title("Cluster resource utilization in Time");
    plt::xlabel("Time (in seconds)");
    plt::ylabel("Resource (in percent)");

    plt::plot(uTimes, utilizationCPU, {{"label", "CPU"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationCPU), {{"label", "CPU AVG at " + std::to_string(averageUtilizationCPU)}});

    plt::plot(uTimes, utilizationMemory, {{"label", "Memory"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationMemory), {{"label", "Memory AVG at " + std::to_string(averageUtilizationMemory)}});

    plt::plot(uTimes, utilizationDisk, {{"label", "Disk"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationDisk), {{"label", "Disk AVG at " + std::to_string(averageUtilizationDisk)}});

    std::for_each(pendingTask.begin(), pendingTask.end(), [](uint64_t& value) { value /= 500; });
    plt::plot(uTimes, pendingTask, {{"label", "Pending task / 500"}});

    plt::legend();
    plt::savefig("../output/" + name + "_cluster_utilization.png");
    plt::show();
}

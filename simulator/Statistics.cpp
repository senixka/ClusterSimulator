#include "Statistics.h"

#include <cmath>
#include <iomanip>

#include "../matplotlibcpp.h"
namespace plt = matplotlibcpp;


void Statistics::UpdateUtilization(uint64_t currentTime, long double usedCPU, long double usedMemory, long double usedDisk) {
    utilizationMeasurementsTime.push_back(currentTime);

    utilizationCPU.push_back(static_cast<float>(usedCPU * 100 / totalAvailableCPU));
    utilizationMemory.push_back(static_cast<float>(usedMemory * 100 / totalAvailableMemory));
    utilizationDisk.push_back(static_cast<float>(usedDisk * 100 / totalAvailableDisk));
}

void Statistics::OnJobSubmitted(uint64_t currentTime, const Job& job) {
    ++jobSubmittedCounter;

    jobStartTime[job.jobID] = currentTime;
    jobUnfinishedTaskCount[job.jobID] = job.pendingTask.size();

    uint64_t minTaskEstimateTime = job.pendingTask.front()->estimate;
    for (const auto& task : job.pendingTask) {
        if (minTaskEstimateTime > task->estimate) {
            minTaskEstimateTime = task->estimate;
        }
    }
    jobMinEstimateTime[job.jobID] = minTaskEstimateTime;
}

void Statistics::OnTaskFinished(uint64_t currentTime, const Task& task) {
    ++taskFinishedCounter;

    if (--jobUnfinishedTaskCount[task.jobID] == 0) {
        jobEndTime[task.jobID] = currentTime;
    }
}

long double countSNP(const std::unordered_map<uint64_t, long double>& jobANP) {
    long double snp = 0;
    for (const auto& [jobID, anp] : jobANP) {
        snp += std::log(anp);
    }
    snp = std::exp(snp / jobANP.size());

    return snp;
}

void Statistics::OnSimulationFinished(uint64_t currentTime) {
    //////////////////// MakeSpan ///////////////////////////

    makeSpanTime = currentTime;

    ////////////////// Average Utilization //////////////////

    long double sumCPU = 0, sumMemory = 0, sumDisk = 0;
    for (size_t i = 0; i < utilizationMeasurementsTime.size(); ++i) {
        sumCPU += utilizationCPU[i];
        sumMemory += utilizationMemory[i];
        sumDisk += utilizationDisk[i];
    }

    averageUtilizationCPU = static_cast<float>(sumCPU / utilizationMeasurementsTime.size());
    averageUtilizationMemory = static_cast<float>(sumMemory / utilizationMeasurementsTime.size());
    averageUtilizationDisk = static_cast<float>(sumDisk / utilizationMeasurementsTime.size());

    ////////////////////// Job's ANP ////////////////////////

    for (const auto& [jobID, endTime] : jobEndTime) {
        jobANP[jobID] = static_cast<long double>(jobMinEstimateTime[jobID]) / (endTime - jobStartTime[jobID]);
    }

    ////////////////////// SNP //////////////////////////////

    long double snp = 0;
    for (const auto& [jobID, anp] : jobANP) {
        snp += std::log(anp);
    }
    snp = std::exp(snp / jobANP.size());

    simulationSNP = static_cast<float>(snp);
}

void Statistics::OnMachineAdded(const Machine& machine) {
    totalAvailableCPU += machine.cpuCapacity;
    totalAvailableMemory += machine.memoryCapacity;
    totalAvailableDisk += machine.diskSpaceCapacity;
}

void Statistics::PrintStatistics() {
    if (utilizationMeasurementsTime.empty()) [[unlikely]] {
        printf("CPU: NO INFO  Memory: NO INFO  Disk: NO INFO\n");
    } else [[likely]] {
        printf("CPU: %2.3f%%  Memory: %2.3f%%  Disk: %2.3f%%  ", utilizationCPU.back(), utilizationMemory.back(), utilizationDisk.back());
        printf("Job submitted: %lu / %lu \t Task finished: %lu / %lu\n", jobSubmittedCounter, nJobInSimulation, taskFinishedCounter, nTaskInSimulation);
    }
}

void Statistics::DumpStatistics() {
    std::cout << std::fixed << std::setprecision(10) << "MakeSpan: " << makeSpanTime << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "SNP: " << simulationSNP << std::endl;

    const auto& uTimes = utilizationMeasurementsTime;

    plt::figure_size(1200, 500);
    plt::title("Cluster resource utilization in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Resource (in percent)");

    plt::plot(uTimes, utilizationCPU, {{"label", "CPU"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationCPU), {{"label", "CPU AVG at " + std::to_string(averageUtilizationCPU)}});

    plt::plot(uTimes, utilizationMemory, {{"label", "Memory"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationMemory), {{"label", "Memory AVG at " + std::to_string(averageUtilizationMemory)}});

    plt::plot(uTimes, utilizationDisk, {{"label", "Disk"}});
    plt::plot(uTimes, std::vector<float>(uTimes.size(), averageUtilizationDisk), {{"label", "Disk AVG at " + std::to_string(averageUtilizationDisk)}});

    plt::legend();
    // plt::savefig("./stat/cluster_utilization.png");
    plt::show();
}

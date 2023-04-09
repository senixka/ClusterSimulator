#include "Statistics.h"

#include <cmath>
#include <iomanip>
#include <cassert>

#include "../matplotlibcpp.h"
namespace plt = matplotlibcpp;


void Statistics::UpdateUtilization(uint64_t currentTime) {
    utilizationMeasurementsTime.push_back(currentTime);

    utilizationCPU.push_back(static_cast<float>(currentUsedCPU * 100 / totalAvailableCPU));
    utilizationMemory.push_back(static_cast<float>(currentUsedMemory * 100 / totalAvailableMemory));
    utilizationDisk.push_back(static_cast<float>(currentUsedDisk * 100 / totalAvailableDisk));
}

void Statistics::OnJobSubmitted(uint64_t currentTime, const Job& job) {
    ++jobSubmittedCounter;

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
    //////////////////// MakeSpan ///////////////////////////
    {
        makeSpanTime = currentTime;
    }

    ////////////////// Average Utilization //////////////////
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
    totalAvailableCPU += machine.cpuCapacity;
    totalAvailableMemory += machine.memoryCapacity;
    totalAvailableDisk += machine.diskSpaceCapacity;
}

void Statistics::PrintStatistics() {
    if (utilizationMeasurementsTime.empty()) [[unlikely]] {
        printf("CPU: NO INFO  Memory: NO INFO  Disk: NO INFO\n");
    } else [[likely]] {
        printf("CPU: %7.3f%%  Memory: %7.3f%%  Disk: %7.3f%%  ", utilizationCPU.back(), utilizationMemory.back(), utilizationDisk.back());
        printf("Job submitted: %lu / %lu \t Task finished: %lu / %lu \t Task working: %lu\n", jobSubmittedCounter, nJobInSimulation, taskFinishedCounter, nTaskInSimulation, currentWorkingTaskCounter);
    }
}

void Statistics::DumpStatistics() {
    std::cout << std::fixed << std::setprecision(10) << "MakeSpan: " << makeSpanTime << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "SNP: " << simulationSNP << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Unfairness: " << simulationUnfairness << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Slowdown: " << simulationSlowdown2Norm << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Min ANP: " << minANP << std::endl;
    std::cout << std::fixed << std::setprecision(10) << "Max ANP: " << maxANP << std::endl;

    //////////////////////////// Utilization //////////////////////////////

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
    plt::savefig("../output/cluster_utilization_1.png");
    plt::show();

    ///////////////////////// Job Completion Time /////////////////////////

    plt::figure_size(1200, 500);
    plt::title("Job completion counter in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Counter");

    std::vector<uint64_t> counter(jobCompletionTime.size());
    for (size_t i = 0; i < counter.size(); ++i) {
        counter[i] = i + 1;
    }
    plt::plot(jobCompletionTime, counter, {{"label", "Job finished"}});

    plt::legend();
    plt::savefig("../output/job_completion_1.png");
    plt::show();

}

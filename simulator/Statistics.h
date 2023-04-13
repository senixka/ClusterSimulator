#pragma once

#include "Job.h"
#include "Machine.h"

#include <cstdint>
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>


class Statistics {
public:
    explicit Statistics(const std::string& simulationName);
    void UpdateStats(uint64_t currentTime);
    void OnJobSubmitted(uint64_t currentTime, const Job& job);
    void OnTaskScheduled(uint64_t currentTime, const Task& task);
    void OnTaskFinished(uint64_t currentTime, const Task& task);
    void OnSimulationFinished(uint64_t currentTime);
    void OnMachineAdded(const Machine& machine);
    void PrintStatistics() const;
    void DumpStatistics();

public:
    uint64_t makeSpanTime_{0};

    long double totalAvailableCPU_{0};
    long double totalAvailableMemory_{0};
    long double totalAvailableDisk_{0};

    long double currentUsedCPU_{0};
    long double currentUsedMemory_{0};
    long double currentUsedDisk_{0};

    float averageUtilizationCPU_{0};
    float averageUtilizationMemory_{0};
    float averageUtilizationDisk_{0};

    std::vector<float> utilizationCPU_;
    std::vector<float> utilizationMemory_;
    std::vector<float> utilizationDisk_;

    std::vector<uint64_t> utilizationMeasurementsTime_;

    std::unordered_map<uint64_t, uint64_t> jobStartTime_;
    std::unordered_map<uint64_t, uint64_t> jobUnfinishedTaskCount_;
    std::unordered_map<uint64_t, uint64_t> jobEndTime_;
    std::unordered_map<uint64_t, uint64_t> jobMinEstimateTime_;

    std::unordered_map<uint64_t, long double> jobANP_;

    uint64_t nJobInSimulation_{0};
    uint64_t jobSubmittedCounter_{0};

    uint64_t nTaskInSimulation_{0};
    uint64_t taskFinishedCounter_{0};

    long double simulationSNP_{0};
    long double simulationUnfairness_{0};
    long double simulationSlowdown2Norm_{0};

    long double minANP_{INT16_MAX};
    long double maxANP_{INT16_MIN};

    std::vector<uint64_t> jobCompletionTime_;

    uint64_t currentWorkingTaskCounter_{0};
    uint64_t currentPendingTaskCounter_{0};

    std::vector<uint64_t> pendingTask_;
    std::vector<uint64_t> workingTask_;

    const std::string simulationName_;
};

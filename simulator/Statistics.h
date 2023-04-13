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
    uint64_t makeSpanTime{0};

    long double totalAvailableCPU{0};
    long double totalAvailableMemory{0};
    long double totalAvailableDisk{0};

    long double currentUsedCPU{0};
    long double currentUsedMemory{0};
    long double currentUsedDisk{0};

    float averageUtilizationCPU{0};
    float averageUtilizationMemory{0};
    float averageUtilizationDisk{0};

    std::vector<float> utilizationCPU;
    std::vector<float> utilizationMemory;
    std::vector<float> utilizationDisk;

    std::vector<uint64_t> utilizationMeasurementsTime;

    std::unordered_map<uint64_t, uint64_t> jobStartTime;
    std::unordered_map<uint64_t, uint64_t> jobUnfinishedTaskCount;
    std::unordered_map<uint64_t, uint64_t> jobEndTime;
    std::unordered_map<uint64_t, uint64_t> jobMinEstimateTime;

    std::unordered_map<uint64_t, long double> jobANP;

    uint64_t nJobInSimulation{0};
    uint64_t jobSubmittedCounter{0};

    uint64_t nTaskInSimulation{0};
    uint64_t taskFinishedCounter{0};

    long double simulationSNP{0};
    long double simulationUnfairness{0};
    long double simulationSlowdown2Norm{0};

    long double minANP{INT16_MAX};
    long double maxANP{INT16_MIN};

    std::vector<uint64_t> jobCompletionTime;

    uint64_t currentWorkingTaskCounter{0};
    uint64_t currentPendingTaskCounter{0};

    std::vector<uint64_t> pendingTask;
    std::vector<uint64_t> workingTask;

    std::string name;

public:
    Statistics(const std::string& sName);
    void UpdateStats(uint64_t currentTime);
    void OnJobSubmitted(uint64_t currentTime, const Job& job);
    void OnTaskScheduled(uint64_t currentTime, const Task& task);
    void OnTaskFinished(uint64_t currentTime, const Task& task);
    void OnSimulationFinished(uint64_t currentTime);
    void OnMachineAdded(const Machine& machine);
    void PrintStatistics();
    void DumpStatistics();
};

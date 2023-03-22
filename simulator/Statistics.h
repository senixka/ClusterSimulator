#ifndef CLUSTERSIMULATOR_STATISTICS_H
#define CLUSTERSIMULATOR_STATISTICS_H

#include "Job.h"
#include "Machine.h"

#include <cstdint>
#include <vector>
#include <unordered_map>


class Statistics {
public:
    uint64_t makeSpanTime{0};

    long double totalAvailableCPU{0};
    long double totalAvailableMemory{0};
    long double totalAvailableDisk{0};

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

    float simulationSNP{0};
    float simulationUnfairness{0};

    std::vector<uint64_t> jobCompletionTime;

public:
    void UpdateUtilization(uint64_t currentTime, long double usedCPU, long double usedMemory, long double usedDisk);
    void OnJobSubmitted(uint64_t currentTime, const Job& job);
    void OnTaskFinished(uint64_t currentTime, const Task& task);
    void OnSimulationFinished(uint64_t currentTime);
    void OnMachineAdded(const Machine& machine);
    void PrintStatistics();
    void DumpStatistics();
};


#endif //CLUSTERSIMULATOR_STATISTICS_H

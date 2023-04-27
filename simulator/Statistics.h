#pragma once

#include "Job.h"
#include "Machine.h"

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>


class Statistics {
public:
    explicit Statistics(const std::string& outputFilePath);
    void UpdateStats(uint64_t currentTime);
    void OnJobSubmitted(uint64_t currentTime, const Job& job);
    void OnTaskScheduled(const Task& task);
    void OnTaskFinished(uint64_t currentTime, const Task& task);
    void OnSimulationFinished(uint64_t currentTime);
    void OnMachineAdded(const Machine& machine);
    void PrintStatistics() const;
    void DumpStatistics();

public:
    uint64_t makeSpanTime_{0};

    uint64_t totalAvailableCPU_{0};
    uint64_t totalAvailableMemory_{0};

    uint64_t currentUsedCPU_{0};
    uint64_t currentUsedMemory_{0};

    std::vector<uint64_t> utilizationCPUNumerator_;
    std::vector<uint64_t> utilizationMemoryNumerator_;
    std::vector<uint64_t> utilizationMeasurementsTime_;

    std::unordered_map<uint64_t, uint64_t> jobStartTime_;
    std::unordered_map<uint64_t, uint64_t> jobEndTime_;

    std::unordered_map<uint64_t, uint64_t> jobUnfinishedTaskCount_;
    std::unordered_map<uint64_t, uint64_t> jobIdealEstimateTime_;

    std::vector<uint64_t> pendingTask_;
    std::vector<uint64_t> workingTask_;

    uint64_t currentWorkingTaskCounter_{0};
    uint64_t currentPendingTaskCounter_{0};
    uint64_t currentUnfinishedJobCounter_{0};

    uint64_t nJobInSimulation_{0};
    uint64_t jobSubmittedCounter_{0};

    uint64_t nTaskInSimulation_{0};
    uint64_t taskFinishedCounter_{0};

    const std::string outputFilePath_;
};

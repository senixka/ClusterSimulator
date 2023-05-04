#pragma once

#include "Job.h"
#include "Machine.h"
#include "Defines.h"

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>


class Statistics {
public:
    explicit Statistics(const std::string& outputFilePath, const std::string& jobManagerName,
                        const std::string& taskManagerName, const std::string& placingStrategyName);
    void UpdateStats(BoundedTimeT currentTime);
    void OnJobSubmitted(BoundedTimeT currentTime, const Job& job);
    void OnTaskScheduled(const Task& task);
    void OnTaskFinished(BoundedTimeT currentTime, const Task& task);
    void OnSimulationFinished(BoundedTimeT currentTime);
    void OnMachineAdded(const Machine& machine);
    void PrintStatistics() const;
    void DumpStatistics() const;

public:
    BoundedTimeT makeSpanTime_{0};

    uint64_t totalAvailableCPU_{0};
    uint64_t totalAvailableMemory_{0};

    uint64_t currentUsedCPU_{0};
    uint64_t currentUsedMemory_{0};

    std::vector<uint64_t> utilizationCPUNumerator_;
    std::vector<uint64_t> utilizationMemoryNumerator_;
    std::vector<BoundedTimeT> utilizationMeasurementsTime_;

    std::unordered_map<JobIdT, BoundedTimeT> jobStartTime_;
    std::unordered_map<JobIdT, BoundedTimeT> jobEndTime_;

    std::unordered_map<JobIdT, size_t> jobUnfinishedTaskCount_;
    std::unordered_map<JobIdT, BoundedTimeT> jobIdealEstimateTime_;

    std::vector<size_t> pendingTask_;
    std::vector<size_t> workingTask_;

    size_t maxPendingTask_{0};

    size_t currentWorkingTaskCounter_{0};
    size_t currentPendingTaskCounter_{0};
    size_t currentUnfinishedJobCounter_{0};

    size_t nJobInSimulation_{0};
    size_t jobSubmittedCounter_{0};

    size_t nTaskInSimulation_{0};
    size_t taskFinishedCounter_{0};

    const std::string outputFilePath_;
    const std::string jobManagerName_;
    const std::string taskManagerName_;
    const std::string placingStrategyName_;
};

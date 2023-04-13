#pragma once

#include "job_manager/FactoryJobManager.h"
#include "task_manager/FactoryTaskManager.h"
#include "placing_strategy/FactoryPlacingStrategy.h"

#include <string>
#include <unordered_map>


class Experiment {
public:
    void Do(const std::string& taskAndJobFilePath, const std::string& machineFilePath,
            const std::string& jobManagerName, const std::string& taskManagerName, const std::string& placingStrategyName) const;

public:
    const std::unordered_map<std::string, JobManagerType> nameToJobManagerType_{
            {"RoundRobin", JobManagerType::RoundRobin},
    };

    const std::unordered_map<std::string, TaskManagerType> nameToTaskManagerType_{
            {"InJobOrder", TaskManagerType::InJobOrder},
    };

    const std::unordered_map<std::string, PlacingStrategyType> nameToPlacingStrategyType_{
            {"Random", PlacingStrategyType::Random},
            {"MinVolume", PlacingStrategyType::MinVolume},
            {"Tetris", PlacingStrategyType::Tetris},
    };
};

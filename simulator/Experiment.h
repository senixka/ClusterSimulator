#pragma once

#include "job_manager/FactoryJobManager.h"
#include "task_manager/FactoryTaskManager.h"
#include "placing_strategy/FactoryPlacingStrategy.h"

#include <string>
#include <unordered_map>


class Experiment {
public:
    Experiment(const std::string& taskAndJobFilePath, const std::string& machineFilePath, const std::string& outputFilePath,
               const std::string& jobManagerName, const std::string& taskManagerName, const std::string& placingStrategyName);

    void Do();

private:
    bool experimentDone_{false};

public:
    const std::string taskAndJobFilePath_;
    const std::string machineFilePath_;
    const std::string outputFilePath_;
    const std::string jobManagerName_;
    const std::string taskManagerName_;
    const std::string placingStrategyName_;
};

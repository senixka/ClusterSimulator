#include "Experiment.h"

#include "Macro.h"
#include "Cluster.h"
#include "Statistics.h"
#include "SchedulerImpl.h"
#include "MachineManager.h"
#include "job_manager/FactoryJobManager.h"
#include "task_manager/FactoryTaskManager.h"
#include "placing_strategy/FactoryPlacingStrategy.h"

#include <cstdlib>
#include <random>
#include <memory>


void Experiment::Do(const std::string& taskAndJobFilePath, const std::string& machineFilePath, const std::string& outputFilePath,
                    const std::string& jobManagerName, const std::string& taskManagerName, const std::string& placingStrategyName) const {
    ASSERT(nameToJobManagerType_.find(jobManagerName) != nameToJobManagerType_.end());
    ASSERT(nameToTaskManagerType_.find(taskManagerName) != nameToTaskManagerType_.end());
    ASSERT(nameToPlacingStrategyType_.find(placingStrategyName) != nameToPlacingStrategyType_.end());
    ASSERT(!taskAndJobFilePath.empty());
    ASSERT(!machineFilePath.empty());
    ASSERT(!outputFilePath.empty());

    const JobManagerType jobManagerType{nameToJobManagerType_.at(jobManagerName)};
    const TaskManagerType taskManagerType{nameToTaskManagerType_.at(taskManagerName)};
    const PlacingStrategyType placingStrategyType{nameToPlacingStrategyType_.at(placingStrategyName)};

    std::shared_ptr<IJobManager> jobManager{FactoryJobManager::Create(jobManagerType)};
    std::shared_ptr<IPlacingStrategy> placingStrategy{FactoryPlacingStrategy::Create(placingStrategyType)};

    auto scheduler = std::make_shared<SchedulerImpl>(placingStrategy);
    auto machineManager = std::make_shared<MachineManager>(machineFilePath);
    auto statistics = std::make_shared<Statistics>(outputFilePath);

    Cluster cluster(taskAndJobFilePath, taskManagerType, jobManager, scheduler, machineManager, statistics);
    cluster.Run();

    statistics->DumpStatistics();
}

#include "Experiment.h"

#include "Macro.h"
#include "Cluster.h"
#include "Registry.h"
#include "Statistics.h"
#include "SchedulerImpl.h"
#include "MachineManager.h"
#include "job_manager/FactoryJobManager.h"
#include "task_manager/FactoryTaskManager.h"
#include "placing_strategy/FactoryPlacingStrategy.h"

#include <cstdlib>
#include <memory>
#include <filesystem>
#include <string_view>


Experiment::Experiment(const std::string& taskAndJobFilePath, const std::string& machineFilePath, const std::string& outputFilePath,
                       const std::string& jobManagerName, const std::string& taskManagerName, const std::string& placingStrategyName)
       : taskAndJobFilePath_(taskAndJobFilePath), machineFilePath_(machineFilePath), outputFilePath_(outputFilePath),
         jobManagerName_(jobManagerName), taskManagerName_(taskManagerName), placingStrategyName_(placingStrategyName) {
    ASSERT(nameToJobManagerType_.find(jobManagerName_) != nameToJobManagerType_.end());
    ASSERT(nameToTaskManagerType_.find(taskManagerName_) != nameToTaskManagerType_.end());
    ASSERT(nameToPlacingStrategyType_.find(placingStrategyName_) != nameToPlacingStrategyType_.end());

    ASSERT(std::filesystem::exists(taskAndJobFilePath_));
    ASSERT(std::filesystem::exists(machineFilePath_));

    std::string_view prefix{outputFilePath_};
    prefix.remove_suffix(prefix.size() - prefix.find_last_of('/', prefix.size()));
    ASSERT(std::filesystem::exists(prefix));
}

void Experiment::Do() {
    ASSERT(!experimentDone_);

    const JobManagerType jobManagerType{nameToJobManagerType_.at(jobManagerName_)};
    const TaskManagerType taskManagerType{nameToTaskManagerType_.at(taskManagerName_)};
    const PlacingStrategyType placingStrategyType{nameToPlacingStrategyType_.at(placingStrategyName_)};

    std::shared_ptr<IJobManager> jobManager{FactoryJobManager::Create(jobManagerType)};
    std::shared_ptr<IPlacingStrategy> placingStrategy{FactoryPlacingStrategy::Create(placingStrategyType)};

    auto scheduler = std::make_shared<SchedulerImpl>(placingStrategy);
    auto machineManager = std::make_shared<MachineManager>(machineFilePath_);
    auto statistics = std::make_shared<Statistics>(outputFilePath_, jobManagerName_,
                                                   taskManagerName_, placingStrategyName_);

    Cluster cluster(taskAndJobFilePath_, taskManagerType, jobManager, scheduler, machineManager, statistics);
    cluster.Run();

    statistics->DumpStatistics();

    experimentDone_ = true;
}

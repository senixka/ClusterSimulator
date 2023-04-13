#include "Cluster.h"
#include "MachineManager.h"
#include "Statistics.h"
#include "SchedulerImpl.h"
#include "job_manager/FactoryJobManager.h"
#include "task_manager/FactoryTaskManager.h"
#include "placing_strategy/FactoryPlacingStrategy.h"

#include <cstdlib>
#include <random>
#include <memory>


void inline Init() {
    std::srand(179);

    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);
}

int main(int /*argc*/, char** /*argv*/) {
    Init();

    // Experiment Random + InJobOrder + RoundRobin
    {
        const std::string taskAndJobFilePath = "../input/job_and_task.txt";
        const std::string machineFilePath = "../input/machine.txt";

        std::shared_ptr<IJobManager> jobManager{FactoryJobManager::Create(JobManagerType::RoundRobin)};
        std::shared_ptr<IPlacingStrategy> placingStrategy{FactoryPlacingStrategy::Create(PlacingStrategyType::Random)};

        auto scheduler = std::make_shared<SchedulerImpl>(placingStrategy);
        auto machineManager = std::make_shared<MachineManager>(machineFilePath);
        auto statistics = std::make_shared<Statistics>("random");

        Cluster cluster(taskAndJobFilePath, TaskManagerType::InJobOrder, jobManager, scheduler, machineManager, statistics);
        cluster.Run();

        statistics->DumpStatistics();
    }

    return 0;
}

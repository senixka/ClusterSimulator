#include "Cluster.h"
#include "MachineManager.h"
#include "Statistics.h"
#include "SchedulerImpl.h"
#include "placing_strategy/Random.h"
#include "job_manager/RoundRobin.h"
#include "task_manager/FactoryTaskManager.h"

#include <cstdlib>
#include <random>


void inline Init() {
    std::srand(179);

    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);
}

int main() {
    Init();

    // Experiment Random
    {
        const std::string taskAndJobFilePath = "../input/job_and_task.txt";
        const std::string machineFilePath = "../input/machine.txt";

        MachineManager machineManager{machineFilePath};

        RoundRobin jobManager;

        Random placingRandom;
        SchedulerImpl scheduler{&placingRandom};

        Statistics statistics{"random"};

        Cluster cluster(taskAndJobFilePath, &machineManager, TaskManagerType::InJobOrder, &jobManager, &scheduler, &statistics);
        cluster.Run();

        statistics.DumpStatistics();
    }

    // Experiment MinVolume
//    {
//        const std::string taskAndJobFilePath = "../input/job_and_task.txt";
//        const std::string machineFilePath = "../input/machine.txt";
//
//        MachineManager machineManager{machineFilePath};
//        SchedulerMinVolume scheduler{};
//        Statistics statistics{"min_volume"};
//
//        Cluster cluster(taskAndJobFilePath, &machineManager, &scheduler, &statistics);
//        cluster.Run();
//
//        statistics.DumpStatistics();
//    }
//
//    // Experiment Tetris
//    {
//        const std::string taskAndJobFilePath = "../input/job_and_task.txt";
//        const std::string machineFilePath = "../input/machine.txt";
//
//        MachineManager machineManager{machineFilePath};
//        SchedulerTetris scheduler{};
//        Statistics statistics{"tetris"};
//
//        Cluster cluster(taskAndJobFilePath, &machineManager, &scheduler, &statistics);
//        cluster.Run();
//
//        statistics.DumpStatistics();
//    }

    return 0;
}

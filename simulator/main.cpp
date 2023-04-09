#include "Cluster.h"
#include "MachineManager.h"
#include "Statistics.h"
#include "SchedulerRandom.h"

#include <cstdlib>
#include <random>

void inline Init() {
    std::srand(179);

    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);
}

int main() {
    Init();

    // Experiment
    {
        const std::string task_and_job = "../input/job_and_task.txt";
        const std::string machine = "../input/machine.txt";

        MachineManager machineManager{machine};
        SchedulerRandom schedulerRandom{};
        Statistics statistics{};

        Cluster cluster(task_and_job, &machineManager, &schedulerRandom, &statistics);
        cluster.Run();

        statistics.DumpStatistics();
    }

    return 0;
}

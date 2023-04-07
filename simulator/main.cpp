#include "Cluster.h"
#include "MachineManager.h"
#include "Statistics.h"
#include "SchedulerRandom.h"

#include <cstdlib>
#include <random>


int main() {
    std::srand(179);

    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);

    {
        MachineManager machineManager{};
        SchedulerRandom schedulerRandom{};
        Statistics statistics{};

        Cluster cluster(&machineManager, &schedulerRandom, &statistics);
        cluster.Run();

        statistics.DumpStatistics();
    }

    return 0;
}

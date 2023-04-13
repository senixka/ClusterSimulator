#include "Experiment.h"


void inline Init() {
    std::srand(179);

    char env[] = "PYTHONMALLOC=malloc";
    putenv(env);
}

int main(int /*argc*/, char** /*argv*/) {
    Init();

    // Experiment RoundRobin + InJobOrder + Random
    {
        Experiment experiment;
        experiment.Do("../input/job_and_task.txt", "../input/machine.txt", "RoundRobin", "InJobOrder", "Random");
    }

    return 0;
}

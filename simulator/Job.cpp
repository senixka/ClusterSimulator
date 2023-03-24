#include "Job.h"


Job::Job(std::istream& in) {
    in >> jobID >> jobTime;

    size_t taskN;
    in >> taskN;

    for (size_t i = 0; i < taskN; ++i) {
        Task* task = new Task();

        task->jobID = jobID;
        in >> task->taskIndex >> task->estimate >> task->cpuRequest >> task->memoryRequest >> task->diskSpaceRequest;

        pendingTask.push_back(task);
    }
}

Job::~Job() {
    for (auto task : pendingTask) {
        delete task;
    }
}

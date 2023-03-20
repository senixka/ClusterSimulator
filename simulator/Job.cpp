#include "Job.h"


Job::Job(std::istream& in) {
    in >> jobTime >> jobID >> user;

    size_t taskN;
    in >> taskN;

    Task task;
    for (size_t i = 0; i < taskN; ++i) {
        in >> task.estimate >> task.cpuRequest >> task.memoryRequest >> task.diskSpaceRequest;
        pendingTask.push_back(task);
    }
}

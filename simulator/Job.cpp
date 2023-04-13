#include "Job.h"


Job::Job(TaskManagerType taskManagerType, std::istream& in)
    : taskManager(FactoryTaskManager::Create(taskManagerType)) {
    in >> jobID >> jobTime;

    size_t taskN;
    in >> taskN;

    for (size_t i = 0; i < taskN; ++i) {
        Task* task = new Task();

        task->jobID = jobID;
        in >> task->taskIndex >> task->estimate >> task->cpuRequest >> task->memoryRequest >> task->diskSpaceRequest;

        taskManager->PutTask(task);
    }
}

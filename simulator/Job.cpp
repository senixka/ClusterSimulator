#include "Job.h"


Job::Job(TaskManagerType taskManagerType, std::istream& in)
    : taskManager_(FactoryTaskManager::Create(taskManagerType)) {
    in >> jobID_ >> jobTime_;

    size_t taskN;
    in >> taskN;

    uint64_t estimate;
    unsigned taskIndex, cpuRequest, memoryRequest;

    for (size_t i = 0; i < taskN; ++i) {
        in >> taskIndex >> estimate >> cpuRequest >> memoryRequest;

        Task* task = new Task(estimate, cpuRequest, memoryRequest, jobID_, taskIndex);
        taskManager_->PutTask(task);
    }

    taskManager_->Sort();
}

Job::~Job() {
    delete taskManager_;
}

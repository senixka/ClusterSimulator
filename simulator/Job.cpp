#include "Job.h"


Job::Job(TaskManagerType taskManagerType, std::istream& in, unsigned jobID)
    : taskManager_(FactoryTaskManager::Create(taskManagerType)), jobID_(jobID) {
    size_t kEntries;
    in >> jobTime_ >> kEntries;

    for (size_t i = 0; i < kEntries; ++i) {
        size_t kTask;
        uint64_t estimate;
        unsigned cpuRequest, memoryRequest;

        in >> kTask >> estimate >> cpuRequest >> memoryRequest;
        taskManager_->NewTasks(kTask, estimate, cpuRequest, memoryRequest, jobID_);
    }

    taskManager_->Sort();
}

Job::~Job() {
    delete taskManager_;
}

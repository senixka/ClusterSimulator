#include "Job.h"

#include <limits>
#include <type_traits>


Job::~Job() {
    delete taskManager_;
}


namespace job_memory_pool {

static_assert(sizeof(std::aligned_storage<sizeof(Job), alignof(Job)>::type) == sizeof(Job));

static boost::object_pool<Job, boost::default_user_allocator_new_delete> pool_;
static unsigned allocCount_ = 0;
static unsigned maxAlloc_ = 0;

} // namespace job_memory_pool


void Job::PrepareMemoryPool(unsigned nJob) {
    job_memory_pool::pool_.set_next_size(nJob);
    job_memory_pool::allocCount_ = 0;
    job_memory_pool::maxAlloc_ = nJob;
    printf("Job memory pool can serve %u allocations\n", job_memory_pool::maxAlloc_);
}

Job* Job::New(TaskManagerType taskManagerType, std::istream& in, JobIdT jobId) {
    ASSERT(job_memory_pool::allocCount_ < job_memory_pool::maxAlloc_);

    Job* ptr = job_memory_pool::pool_.construct();
    ASSERT(ptr != nullptr);
    ++job_memory_pool::allocCount_;

    ptr->jobId_ = jobId;
    ptr->taskManager_ = FactoryTaskManager::Create(taskManagerType);

    uint64_t jobTime, nEntries;
    in >> jobTime >> nEntries;

    ASSERT(jobTime < std::numeric_limits<BoundedTimeT>::max());
    ASSERT(nEntries < std::numeric_limits<unsigned>::max())

    ptr->jobTime_ = static_cast<BoundedTimeT>(jobTime);

    for (uint64_t i = 0; i < nEntries; ++i) {
        uint64_t nTask, estimate, cpuRequest, memoryRequest;
        in >> nTask >> estimate >> cpuRequest >> memoryRequest;

        ASSERT(nTask < std::numeric_limits<unsigned>::max());
        ASSERT(estimate <= std::numeric_limits<BoundedTimeT>::max());
        ASSERT(cpuRequest < std::numeric_limits<ResourceT>::max());
        ASSERT(memoryRequest < std::numeric_limits<ResourceT>::max());

        ptr->taskManager_->NewTasks(
                static_cast<unsigned>(nTask),
                static_cast<BoundedTimeT>(estimate),
                static_cast<ResourceT>(cpuRequest),
                static_cast<ResourceT>(memoryRequest),
                jobId
        );
    }

    ptr->taskManager_->Sort();

    return ptr;
}

void Job::Delete(Job* /*ptr*/) {
}

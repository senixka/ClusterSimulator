#include "Task.h"

#include <type_traits>


namespace task_memory_pool {

static_assert(sizeof(std::aligned_storage<sizeof(Task), alignof(Task)>::type) == sizeof(Task));

static boost::object_pool<Task, boost::default_user_allocator_new_delete> pool_;
static unsigned allocCount_ = 0;
static unsigned maxAlloc_ = 0;

} // namespace task_memory_pool


void Task::PrepareMemoryPool(unsigned nTask) {
    task_memory_pool::pool_.set_next_size(nTask);
    task_memory_pool::allocCount_ = 0;
    task_memory_pool::maxAlloc_ = nTask;
    printf("Task memory pool can serve %u allocations\n", task_memory_pool::maxAlloc_);
}

Task* Task::New(BoundedTimeT estimate, ResourceT cpuRequest, ResourceT memoryRequest, JobIdT jobId) {
    ASSERT(task_memory_pool::allocCount_ < task_memory_pool::maxAlloc_);

    ASSERT(1 <= cpuRequest);
    ASSERT(cpuRequest <= MACHINE_MAX_POSSIBLE_CPU);

    ASSERT(1 <= memoryRequest);
    ASSERT(memoryRequest <= MACHINE_MAX_POSSIBLE_MEMORY);

    Task* ptr = task_memory_pool::pool_.construct();
    ASSERT(ptr != nullptr);
    ++task_memory_pool::allocCount_;

    ptr->estimate_ = estimate;
    ptr->cpuRequest_ = cpuRequest;
    ptr->memoryRequest_ = memoryRequest;
    ptr->jobId_ = jobId;

    return ptr;
}

void Task::Delete(Task* /*ptr*/) {
}

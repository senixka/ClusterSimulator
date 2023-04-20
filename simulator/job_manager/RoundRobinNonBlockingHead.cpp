#include "RoundRobinNonBlockingHead.h"

#include "../Macro.h"


namespace job_manager {

void RoundRobinNonBlockingHead::PutJob(Job* job) {
    jobs_.push_back(job);
}

Job* RoundRobinNonBlockingHead::GetJob() {
    ASSERT(!jobs_.empty());

    Job* job = jobs_.front();
    jobs_.pop_front();
    return job;
}

void RoundRobinNonBlockingHead::ReturnJob(Job* job, bool /*isModified*/) {
    if (job->taskManager_->TaskCount() == 0) {
        delete job;
        return;
    }

    jobs_.push_back(job);
}

size_t RoundRobinNonBlockingHead::JobCount() {
    return jobs_.size();
}

void RoundRobinNonBlockingHead::NewSchedulingCycle() {
}

RoundRobinNonBlockingHead::~RoundRobinNonBlockingHead() {
    for (Job* job : jobs_) {
        delete job;
    }
}

} // namespace job_manager

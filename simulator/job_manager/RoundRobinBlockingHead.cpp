#include "RoundRobinBlockingHead.h"

#include "../Macro.h"


namespace job_manager {

void RoundRobinBlockingHead::PutJob(Job* job) {
    jobs_.push_back(job);
}

Job* RoundRobinBlockingHead::GetJob() {
    ASSERT(!jobs_.empty());

    Job* job = jobs_.front();
    jobs_.pop_front();
    return job;
}

void RoundRobinBlockingHead::ReturnJob(Job* job, bool isModified) {
    if (job->taskManager_->TaskCount() == 0) {
        delete job;
        return;
    }

    if (isModified) {
        jobs_.push_back(job);
    } else {
        jobs_.push_front(job);
    }
}

size_t RoundRobinBlockingHead::JobCount() {
    return jobs_.size();
}

void RoundRobinBlockingHead::NewSchedulingCycle() {
}

RoundRobinBlockingHead::~RoundRobinBlockingHead() {
    for (Job* job : jobs_) {
        delete job;
    }
}

} // namespace job_manager

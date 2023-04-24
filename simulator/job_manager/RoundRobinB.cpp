#include "RoundRobinB.h"

#include "../Macro.h"


namespace job_manager {

void RoundRobinB::PutJob(Job* job) {
    jobs_.push_back(job);
}

Job* RoundRobinB::GetJob() {
    ASSERT(!jobs_.empty());

    Job* job = jobs_.front();
    jobs_.pop_front();
    return job;
}

void RoundRobinB::ReturnJob(Job* job, bool isModified) {
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

size_t RoundRobinB::JobCount() {
    return jobs_.size();
}

void RoundRobinB::NewSchedulingCycle() {
}

RoundRobinB::~RoundRobinB() {
    for (Job* job : jobs_) {
        delete job;
    }
}

} // namespace job_manager

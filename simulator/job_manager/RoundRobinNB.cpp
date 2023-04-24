#include "RoundRobinNB.h"

#include "../Macro.h"


namespace job_manager {

void RoundRobinNB::PutJob(Job* job) {
    jobs_.push_back(job);
}

Job* RoundRobinNB::GetJob() {
    ASSERT(!jobs_.empty());

    Job* job = jobs_.front();
    jobs_.pop_front();
    return job;
}

void RoundRobinNB::ReturnJob(Job* job, bool /*isModified*/) {
    if (job->taskManager_->TaskCount() == 0) {
        delete job;
        return;
    }

    jobs_.push_back(job);
}

size_t RoundRobinNB::JobCount() {
    return jobs_.size();
}

void RoundRobinNB::NewSchedulingCycle() {
}

RoundRobinNB::~RoundRobinNB() {
    for (Job* job : jobs_) {
        delete job;
    }
}

} // namespace job_manager

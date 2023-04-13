#include "RoundRobin.h"

#include "../Macro.h"


void RoundRobin::PutJob(Job* job) {
    jobs_.push_back(job);
}

Job* RoundRobin::GetJob() {
    ASSERT(!jobs_.empty());

    Job* job = jobs_.front();
    jobs_.pop_front();
    return job;
}

void RoundRobin::ReturnJob(Job* job, bool isModified) {
    if (job->taskManager_->TaskCount() == 0) {
        delete job;
        return;
    }

    if (isModified) {
        jobs_.push_back(job);
    } else {
        //jobs.push_front(job);
        jobs_.push_back(job);
    }
}

size_t RoundRobin::JobCount() {
    return jobs_.size();
}

RoundRobin::~RoundRobin() {
    for (Job* job : jobs_) {
        delete job;
    }
}

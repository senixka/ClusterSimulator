#include "RoundRobin.h"

#include "../Macro.h"


void RoundRobin::PutJob(Job* job) {
    jobs.push_back(job);
}

Job* RoundRobin::GetJob() {
    ASSERT(!jobs.empty());

    Job* job = jobs.front();
    jobs.pop_front();
    return job;
}

void RoundRobin::ReturnJob(Job* job, bool isModified) {
    if (job->taskManager->TaskCount() == 0) {
        delete job;
        return;
    }

    if (isModified) {
        jobs.push_back(job);
    } else {
        //jobs.push_front(job);
        jobs.push_back(job);
    }
}

size_t RoundRobin::JobCount() {
    return jobs.size();
}

RoundRobin::~RoundRobin() {
    for (Job* job : jobs) {
        delete job;
    }
}

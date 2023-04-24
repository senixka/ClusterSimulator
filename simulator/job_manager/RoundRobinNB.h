#pragma once

#include "IJobManager.h"
#include "../Job.h"

#include <list>


namespace job_manager {

class RoundRobinNB : public IJobManager {
public:
    void PutJob(Job* job) override;

    Job* GetJob() override;
    void ReturnJob(Job* job, bool isModified) override;

    size_t JobCount() override;
    void NewSchedulingCycle() override;

    ~RoundRobinNB();

private:
    std::list<Job*> jobs_;
};

} // namespace job_manager

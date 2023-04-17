#pragma once

#include "IJobManager.h"
#include "../Job.h"

#include <list>


class RoundRobinBlockingHead : public IJobManager {
public:
    void PutJob(Job* job) override;

    Job* GetJob() override;
    void ReturnJob(Job* job, bool isModified) override;

    size_t JobCount() override;

    ~RoundRobinBlockingHead();

private:
    std::list<Job*> jobs_;
};

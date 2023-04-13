#ifndef CLUSTERSIMULATOR_ROUNDROBIN_H
#define CLUSTERSIMULATOR_ROUNDROBIN_H

#include "IJobManager.h"

#include "../Job.h"

#include <list>


class RoundRobin : public IJobManager {
public:
    void PutJob(Job* job) override;

    Job* GetJob() override;
    void ReturnJob(Job* job, bool isModified) override;

    size_t JobCount() override;

    ~RoundRobin();

private:
    std::list<Job*> jobs;
};


#endif //CLUSTERSIMULATOR_ROUNDROBIN_H

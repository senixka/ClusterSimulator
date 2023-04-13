#ifndef CLUSTERSIMULATOR_IJOBMANAGER_H
#define CLUSTERSIMULATOR_IJOBMANAGER_H

#include "../Job.h"


class IJobManager {
public:
    virtual void PutJob(Job* job) = 0;

    virtual Job* GetJob() = 0;
    virtual void ReturnJob(Job* job, bool isModified) = 0;

    virtual size_t JobCount() = 0;

    virtual ~IJobManager() = default;
};


#endif //CLUSTERSIMULATOR_IJOBMANAGER_H

#pragma once

#include "../Job.h"


struct IJobManager {
    virtual void PutJob(Job* job) = 0;

    virtual Job* GetJob() = 0;
    virtual void ReturnJob(Job* job, bool isModified) = 0;

    virtual void NewSchedulingCycle() = 0;
    virtual bool IsThereSomethingElse() = 0;

    virtual ~IJobManager() = default;
};

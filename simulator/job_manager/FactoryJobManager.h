#pragma once

#include "IJobManager.h"


enum JobManagerType {
    RoundRobin = 0,
};


class FactoryJobManager {
public:
    static IJobManager* Create(JobManagerType jobManagerType);
};

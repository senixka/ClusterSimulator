#pragma once

#include "IJobManager.h"
#include "../Registry.h"


class FactoryJobManager {
public:
    static IJobManager* Create(JobManagerType jobManagerType);
};

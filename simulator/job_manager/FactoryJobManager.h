#pragma once

#include "IJobManager.h"
#include "../Registry.h"


struct FactoryJobManager {
    static IJobManager* Create(JobManagerType jobManagerType);
};

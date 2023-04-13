#include "FactoryJobManager.h"

#include "RoundRobin.h"
#include "../Macro.h"


IJobManager *FactoryJobManager::Create(JobManagerType jobManagerType) {
    switch (jobManagerType) {
        case JobManagerType::RoundRobin:
            return (new (class RoundRobin){});

        default:
            UNREACHABLE("FactoryJobManager");
    }
}

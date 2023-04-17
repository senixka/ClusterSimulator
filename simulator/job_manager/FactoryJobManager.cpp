#include "FactoryJobManager.h"

#include "RoundRobinBlockingHead.h"
#include "RoundRobinNonBlockingHead.h"
#include "../Macro.h"


IJobManager *FactoryJobManager::Create(JobManagerType jobManagerType) {
    switch (jobManagerType) {
        case JobManagerType::RoundRobinBlockingHead:
            return (new (class RoundRobinBlockingHead){});

        case JobManagerType::RoundRobinNonBlockingHead:
            return (new (class RoundRobinNonBlockingHead){});

        default:
            UNREACHABLE("FactoryJobManager");
    }
}

#include "FactoryJobManager.h"

#include "RoundRobinBlockingHead.h"
#include "RoundRobinNonBlockingHead.h"
#include "MinJobSubmitTimeFirstNonBlocking.h"
#include "MaxJobSubmitTimeFirstNonBlocking.h"
#include "MinSumTaskEstimateFirstNonBlocking.h"
#include "MaxSumTaskEstimateFirstNonBlocking.h"
#include "MinTaskCountFirstNonBlocking.h"
#include "MaxTaskCountFirstNonBlocking.h"

#include "../Macro.h"


IJobManager *FactoryJobManager::Create(JobManagerType jobManagerType) {
    switch (jobManagerType) {
        case JobManagerType::RoundRobinBlockingHead:
            return (new job_manager::RoundRobinBlockingHead{});

        case JobManagerType::RoundRobinNonBlockingHead:
            return (new job_manager::RoundRobinNonBlockingHead{});


        case JobManagerType::MinJobSubmitTimeFirstNonBlocking:
            return (new job_manager::MinJobSubmitTimeFirstNonBlocking{});

        case JobManagerType::MaxJobSubmitTimeFirstNonBlocking:
            return (new job_manager::MaxJobSubmitTimeFirstNonBlocking{});


        case JobManagerType::MinSumTaskEstimateFirstNonBlocking:
            return (new job_manager::MinSumTaskEstimateFirstNonBlocking{});

        case JobManagerType::MaxSumTaskEstimateFirstNonBlocking:
            return (new job_manager::MaxSumTaskEstimateFirstNonBlocking{});


        case JobManagerType::MinTaskCountFirstNonBlocking:
            return (new job_manager::MinTaskCountFirstNonBlocking{});

        case JobManagerType::MaxTaskCountFirstNonBlocking:
            return (new job_manager::MaxTaskCountFirstNonBlocking {});


        default:
            UNREACHABLE("FactoryJobManager");
    }
}

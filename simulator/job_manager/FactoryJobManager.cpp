#include "FactoryJobManager.h"

#include "RoundRobinB.h"
#include "RoundRobinNB.h"
#include "MinJobSubmitTimeNB.h"
#include "MaxJobSubmitTimeNB.h"
#include "MinSumTaskEstimateNB.h"
#include "MaxSumTaskEstimateNB.h"
#include "MinTaskCountNB.h"
#include "MaxTaskCountNB.h"

#include "../Macro.h"


IJobManager *FactoryJobManager::Create(JobManagerType jobManagerType) {
    switch (jobManagerType) {
        case JobManagerType::RoundRobinB:
            return (new job_manager::RoundRobinB{});

        case JobManagerType::RoundRobinNB:
            return (new job_manager::RoundRobinNB{});


        case JobManagerType::MinJobSubmitTimeNB:
            return (new job_manager::MinJobSubmitTimeNB{});

        case JobManagerType::MaxJobSubmitTimeNB:
            return (new job_manager::MaxJobSubmitTimeNB{});


        case JobManagerType::MinSumTaskEstimateNB:
            return (new job_manager::MinSumTaskEstimateNB{});

        case JobManagerType::MaxSumTaskEstimateNB:
            return (new job_manager::MaxSumTaskEstimateNB{});


        case JobManagerType::MinTaskCountNB:
            return (new job_manager::MinTaskCountNB{});

        case JobManagerType::MaxTaskCountNB:
            return (new job_manager::MaxTaskCountNB {});


        default:
            UNREACHABLE("FactoryJobManager");
    }
}

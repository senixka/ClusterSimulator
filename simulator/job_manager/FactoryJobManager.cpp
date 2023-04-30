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
        case JobManagerType::RoundRobinB_1:
            return (new job_manager::RoundRobinB_1{});

        case JobManagerType::RoundRobinB_10:
            return (new job_manager::RoundRobinB_10{});

        case JobManagerType::RoundRobinB_100:
            return (new job_manager::RoundRobinB_100{});


        case JobManagerType::RoundRobinNB_1:
            return (new job_manager::RoundRobinNB_1{});

        case JobManagerType::RoundRobinNB_10:
            return (new job_manager::RoundRobinNB_10{});

        case JobManagerType::RoundRobinNB_100:
            return (new job_manager::RoundRobinNB_100{});


        case JobManagerType::MinJobSubmitTimeNB_1:
            return (new job_manager::MinJobSubmitTimeNB_1{});

        case JobManagerType::MinJobSubmitTimeNB_10:
            return (new job_manager::MinJobSubmitTimeNB_10{});

        case JobManagerType::MinJobSubmitTimeNB_100:
            return (new job_manager::MinJobSubmitTimeNB_100{});


        case JobManagerType::MaxJobSubmitTimeNB_1:
            return (new job_manager::MaxJobSubmitTimeNB_1{});

        case JobManagerType::MaxJobSubmitTimeNB_10:
            return (new job_manager::MaxJobSubmitTimeNB_10{});

        case JobManagerType::MaxJobSubmitTimeNB_100:
            return (new job_manager::MaxJobSubmitTimeNB_100{});


        case JobManagerType::MinSumTaskEstimateNB_1:
            return (new job_manager::MinSumTaskEstimateNB_1{});

        case JobManagerType::MinSumTaskEstimateNB_10:
            return (new job_manager::MinSumTaskEstimateNB_10{});

        case JobManagerType::MinSumTaskEstimateNB_100:
            return (new job_manager::MinSumTaskEstimateNB_100{});


        case JobManagerType::MaxSumTaskEstimateNB_1:
            return (new job_manager::MaxSumTaskEstimateNB_1{});

        case JobManagerType::MaxSumTaskEstimateNB_10:
            return (new job_manager::MaxSumTaskEstimateNB_10{});

        case JobManagerType::MaxSumTaskEstimateNB_100:
            return (new job_manager::MaxSumTaskEstimateNB_100{});


        case JobManagerType::MinTaskCountNB_1:
            return (new job_manager::MinTaskCountNB_1{});

        case JobManagerType::MinTaskCountNB_10:
            return (new job_manager::MinTaskCountNB_10{});

        case JobManagerType::MinTaskCountNB_100:
            return (new job_manager::MinTaskCountNB_100{});


        case JobManagerType::MaxTaskCountNB_1:
            return (new job_manager::MaxTaskCountNB_1{});

        case JobManagerType::MaxTaskCountNB_10:
            return (new job_manager::MaxTaskCountNB_10{});

        case JobManagerType::MaxTaskCountNB_100:
            return (new job_manager::MaxTaskCountNB_100{});


        default:
            UNREACHABLE("FactoryJobManager");
    }
}

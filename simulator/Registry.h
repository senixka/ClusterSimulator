#pragma once

#include <string>
#include <unordered_map>


enum class JobManagerType {
    RoundRobinNB_1,
    RoundRobinNB_10,
    RoundRobinNB_100,

    RoundRobinB_1,
    RoundRobinB_10,
    RoundRobinB_100,

    MinJobSubmitTimeNB_1,
    MinJobSubmitTimeNB_10,
    MinJobSubmitTimeNB_100,

    MaxJobSubmitTimeNB_1,
    MaxJobSubmitTimeNB_10,
    MaxJobSubmitTimeNB_100,

    MinSumTaskEstimateNB_1,
    MinSumTaskEstimateNB_10,
    MinSumTaskEstimateNB_100,

    MaxSumTaskEstimateNB_1,
    MaxSumTaskEstimateNB_10,
    MaxSumTaskEstimateNB_100,

    MinTaskCountNB_1,
    MinTaskCountNB_10,
    MinTaskCountNB_100,

    MaxTaskCountNB_1,
    MaxTaskCountNB_10,
    MaxTaskCountNB_100,
};

static const std::unordered_map<std::string, JobManagerType> nameToJobManagerType_{
        {"RoundRobinB_1", JobManagerType::RoundRobinB_1},
        {"RoundRobinB_10", JobManagerType::RoundRobinB_10},
        {"RoundRobinB_100", JobManagerType::RoundRobinB_100},

        {"RoundRobinNB_1", JobManagerType::RoundRobinNB_1},
        {"RoundRobinNB_10", JobManagerType::RoundRobinNB_10},
        {"RoundRobinNB_100", JobManagerType::RoundRobinNB_100},

        {"MinJobSubmitTimeNB_1", JobManagerType::MinJobSubmitTimeNB_1},
        {"MinJobSubmitTimeNB_10", JobManagerType::MinJobSubmitTimeNB_10},
        {"MinJobSubmitTimeNB_100", JobManagerType::MinJobSubmitTimeNB_100},

        {"MaxJobSubmitTimeNB_1", JobManagerType::MaxJobSubmitTimeNB_1},
        {"MaxJobSubmitTimeNB_10", JobManagerType::MaxJobSubmitTimeNB_10},
        {"MaxJobSubmitTimeNB_100", JobManagerType::MaxJobSubmitTimeNB_100},

        {"MinSumTaskEstimateNB_1", JobManagerType::MinSumTaskEstimateNB_1},
        {"MinSumTaskEstimateNB_10", JobManagerType::MinSumTaskEstimateNB_10},
        {"MinSumTaskEstimateNB_100", JobManagerType::MinSumTaskEstimateNB_100},

        {"MaxSumTaskEstimateNB_1", JobManagerType::MaxSumTaskEstimateNB_1},
        {"MaxSumTaskEstimateNB_10", JobManagerType::MaxSumTaskEstimateNB_10},
        {"MaxSumTaskEstimateNB_100", JobManagerType::MaxSumTaskEstimateNB_100},

        {"MinTaskCountNB_1", JobManagerType::MinTaskCountNB_1},
        {"MinTaskCountNB_10", JobManagerType::MinTaskCountNB_10},
        {"MinTaskCountNB_100", JobManagerType::MinTaskCountNB_100},

        {"MaxTaskCountNB_1", JobManagerType::MaxTaskCountNB_1},
        {"MaxTaskCountNB_10", JobManagerType::MaxTaskCountNB_10},
        {"MaxTaskCountNB_100", JobManagerType::MaxTaskCountNB_100},
};


enum class TaskManagerType {
    RoundRobinNB,
    MinTaskTimeNB,
    MaxTaskTimeNB,
};

static const std::unordered_map<std::string, TaskManagerType> nameToTaskManagerType_{
        {"RoundRobinNB", TaskManagerType::RoundRobinNB},
        {"MinTaskTimeNB", TaskManagerType::MinTaskTimeNB},
        {"MaxTaskTimeNB", TaskManagerType::MaxTaskTimeNB},
};


enum class PlacingStrategyType {
    Tetris_NoPartition,
    Tetris_UsedFirst,
    Tetris_UnusedFirst,

    Random_NoPartition,
    Random_UsedFirst,
    Random_UnusedFirst,

    MinCPU_NoPartition,
    MinCPU_UsedFirst,
    MinCPU_UnusedFirst,

    MaxCPU_NoPartition,
    MaxCPU_UsedFirst,
    MaxCPU_UnusedFirst,

    MinSum_NoPartition,
    MinSum_UsedFirst,
    MinSum_UnusedFirst,

    MaxSum_NoPartition,
    MaxSum_UsedFirst,
    MaxSum_UnusedFirst,

    MinMemory_NoPartition,
    MinMemory_UsedFirst,
    MinMemory_UnusedFirst,

    MaxMemory_NoPartition,
    MaxMemory_UsedFirst,
    MaxMemory_UnusedFirst,

    MinVolume_NoPartition,
    MinVolume_UsedFirst,
    MinVolume_UnusedFirst,

    MaxVolume_NoPartition,
    MaxVolume_UsedFirst,
    MaxVolume_UnusedFirst,

    MinTaskCount_NoPartition,
    MinTaskCount_UsedFirst,
    MinTaskCount_UnusedFirst,

    MaxTaskCount_NoPartition,
    MaxTaskCount_UsedFirst,
    MaxTaskCount_UnusedFirst,

    MinMachineIndex_NoPartition,
    MinMachineIndex_UsedFirst,
    MinMachineIndex_UnusedFirst,
};

static const std::unordered_map<std::string, PlacingStrategyType> nameToPlacingStrategyType_{
        {"Tetris_NoPartition", PlacingStrategyType::Tetris_NoPartition},
        {"Tetris_UsedFirst", PlacingStrategyType::Tetris_UsedFirst},
        {"Tetris_UnusedFirst", PlacingStrategyType::Tetris_UnusedFirst},

        {"Random_NoPartition", PlacingStrategyType::Random_NoPartition},
        {"Random_UsedFirst", PlacingStrategyType::Random_UsedFirst},
        {"Random_UnusedFirst", PlacingStrategyType::Random_UnusedFirst},

        {"MinCPU_NoPartition", PlacingStrategyType::MinCPU_NoPartition},
        {"MinCPU_UsedFirst", PlacingStrategyType::MinCPU_UsedFirst},
        {"MinCPU_UnusedFirst", PlacingStrategyType::MinCPU_UnusedFirst},

        {"MaxCPU_NoPartition", PlacingStrategyType::MaxCPU_NoPartition},
        {"MaxCPU_UsedFirst", PlacingStrategyType::MaxCPU_UsedFirst},
        {"MaxCPU_UnusedFirst", PlacingStrategyType::MaxCPU_UnusedFirst},

        {"MinSum_NoPartition", PlacingStrategyType::MinSum_NoPartition},
        {"MinSum_UsedFirst", PlacingStrategyType::MinSum_UsedFirst},
        {"MinSum_UnusedFirst", PlacingStrategyType::MinSum_UnusedFirst},

        {"MaxSum_NoPartition", PlacingStrategyType::MaxSum_NoPartition},
        {"MaxSum_UsedFirst", PlacingStrategyType::MaxSum_UsedFirst},
        {"MaxSum_UnusedFirst", PlacingStrategyType::MaxSum_UnusedFirst},

        {"MinMemory_NoPartition", PlacingStrategyType::MinMemory_NoPartition},
        {"MinMemory_UsedFirst", PlacingStrategyType::MinMemory_UsedFirst},
        {"MinMemory_UnusedFirst", PlacingStrategyType::MinMemory_UnusedFirst},

        {"MaxMemory_NoPartition", PlacingStrategyType::MaxMemory_NoPartition},
        {"MaxMemory_UsedFirst", PlacingStrategyType::MaxMemory_UsedFirst},
        {"MaxMemory_UnusedFirst", PlacingStrategyType::MaxMemory_UnusedFirst},

        {"MinVolume_NoPartition", PlacingStrategyType::MinVolume_NoPartition},
        {"MinVolume_UsedFirst", PlacingStrategyType::MinVolume_UsedFirst},
        {"MinVolume_UnusedFirst", PlacingStrategyType::MinVolume_UnusedFirst},

        {"MaxVolume_NoPartition", PlacingStrategyType::MaxVolume_NoPartition},
        {"MaxVolume_UsedFirst", PlacingStrategyType::MaxVolume_UsedFirst},
        {"MaxVolume_UnusedFirst", PlacingStrategyType::MaxVolume_UnusedFirst},

        {"MinTaskCount_NoPartition", PlacingStrategyType::MinTaskCount_NoPartition},
        {"MinTaskCount_UsedFirst", PlacingStrategyType::MinTaskCount_UsedFirst},
        {"MinTaskCount_UnusedFirst", PlacingStrategyType::MinTaskCount_UnusedFirst},

        {"MaxTaskCount_NoPartition", PlacingStrategyType::MaxTaskCount_NoPartition},
        {"MaxTaskCount_UsedFirst", PlacingStrategyType::MaxTaskCount_UsedFirst},
        {"MaxTaskCount_UnusedFirst", PlacingStrategyType::MaxTaskCount_UnusedFirst},

        {"MinMachineIndex_NoPartition", PlacingStrategyType::MinMachineIndex_NoPartition},
        {"MinMachineIndex_UsedFirst", PlacingStrategyType::MinMachineIndex_UsedFirst},
        {"MinMachineIndex_UnusedFirst", PlacingStrategyType::MinMachineIndex_UnusedFirst},
};

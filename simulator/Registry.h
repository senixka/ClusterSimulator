#pragma once

#include <string>
#include <unordered_map>


enum class JobManagerType {
    RoundRobinNB,
    RoundRobinB,

    MinJobSubmitTimeNB,
    MaxJobSubmitTimeNB,

    MinSumTaskEstimateNB,
    MaxSumTaskEstimateNB,

    MinTaskCountNB,
    MaxTaskCountNB,
};

static const std::unordered_map<std::string, JobManagerType> nameToJobManagerType_{
        {"RoundRobinB", JobManagerType::RoundRobinB},
        {"RoundRobinNB", JobManagerType::RoundRobinNB},

        {"MinJobSubmitTimeNB", JobManagerType::MinJobSubmitTimeNB},
        {"MaxJobSubmitTimeNB", JobManagerType::MaxJobSubmitTimeNB},

        {"MinSumTaskEstimateNB", JobManagerType::MinSumTaskEstimateNB},
        {"MaxSumTaskEstimateNB", JobManagerType::MaxSumTaskEstimateNB},

        {"MinTaskCountNB", JobManagerType::MinTaskCountNB},
        {"MaxTaskCountNB", JobManagerType::MaxTaskCountNB},
};


enum class TaskManagerType {
    InJobOrderB,
    MinTaskTimeB,
    MaxTaskTimeB,
};

static const std::unordered_map<std::string, TaskManagerType> nameToTaskManagerType_{
        {"InJobOrderB", TaskManagerType::InJobOrderB},
        {"MinTaskTimeB", TaskManagerType::MinTaskTimeB},
        {"MaxTaskTimeB", TaskManagerType::MaxTaskTimeB},
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

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
    Random_NoPartition,
    Random_UsedFirst,
    Random_UnusedFirst,

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

    Tetris_NoPartition,
    Tetris_UsedFirst,
    Tetris_UnusedFirst,
};

static const std::unordered_map<std::string, PlacingStrategyType> nameToPlacingStrategyType_{
        {"Random_NoPartition", PlacingStrategyType::Random_NoPartition},
        {"Random_UsedFirst", PlacingStrategyType::Random_UsedFirst},
        {"Random_UnusedFirst", PlacingStrategyType::Random_UnusedFirst},

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

        {"Tetris_NoPartition", PlacingStrategyType::Tetris_NoPartition},
        {"Tetris_UsedFirst", PlacingStrategyType::Tetris_UsedFirst},
        {"Tetris_UnusedFirst", PlacingStrategyType::Tetris_UnusedFirst},
};

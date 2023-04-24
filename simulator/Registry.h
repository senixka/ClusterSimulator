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
    InJobOrder,
    MinTaskTimeB,
    MaxTaskTimeB,
};

static const std::unordered_map<std::string, TaskManagerType> nameToTaskManagerType_{
        {"InJobOrder", TaskManagerType::InJobOrder},
        {"MinTaskTimeB", TaskManagerType::MinTaskTimeB},
        {"MaxTaskTimeB", TaskManagerType::MaxTaskTimeB},
};


enum class PlacingStrategyType {
    Random_NoReorder,
    Random_UsedFirst,
    Random_UnusedFirst,

    MinVolume_NoReorder,
    MinVolume_UsedFirst,
    MinVolume_UnusedFirst,

    MaxVolume_NoReorder,
    MaxVolume_UsedFirst,
    MaxVolume_UnusedFirst,

    MinTaskCount_NoReorder,
    MinTaskCount_UsedFirst,
    MinTaskCount_UnusedFirst,

    MaxTaskCount_NoReorder,
    MaxTaskCount_UsedFirst,
    MaxTaskCount_UnusedFirst,

    MinMachineIndex_NoReorder,
    MinMachineIndex_UsedFirst,
    MinMachineIndex_UnusedFirst,

    Tetris_NoReorder,
    Tetris_UsedFirst,
    Tetris_UnusedFirst,
};

static const std::unordered_map<std::string, PlacingStrategyType> nameToPlacingStrategyType_{
        {"Random_NoReorder", PlacingStrategyType::Random_NoReorder},
        {"Random_UsedFirst", PlacingStrategyType::Random_UsedFirst},
        {"Random_UnusedFirst", PlacingStrategyType::Random_UnusedFirst},

        {"MinVolume_NoReorder", PlacingStrategyType::MinVolume_NoReorder},
        {"MinVolume_UsedFirst", PlacingStrategyType::MinVolume_UsedFirst},
        {"MinVolume_UnusedFirst", PlacingStrategyType::MinVolume_UnusedFirst},

        {"MaxVolume_NoReorder", PlacingStrategyType::MaxVolume_NoReorder},
        {"MaxVolume_UsedFirst", PlacingStrategyType::MaxVolume_UsedFirst},
        {"MaxVolume_UnusedFirst", PlacingStrategyType::MaxVolume_UnusedFirst},

        {"MinTaskCount_NoReorder", PlacingStrategyType::MinTaskCount_NoReorder},
        {"MinTaskCount_UsedFirst", PlacingStrategyType::MinTaskCount_UsedFirst},
        {"MinTaskCount_UnusedFirst", PlacingStrategyType::MinTaskCount_UnusedFirst},

        {"MaxTaskCount_NoReorder", PlacingStrategyType::MaxTaskCount_NoReorder},
        {"MaxTaskCount_UsedFirst", PlacingStrategyType::MaxTaskCount_UsedFirst},
        {"MaxTaskCount_UnusedFirst", PlacingStrategyType::MaxTaskCount_UnusedFirst},

        {"MinMachineIndex_NoReorder", PlacingStrategyType::MinMachineIndex_NoReorder},
        {"MinMachineIndex_UsedFirst", PlacingStrategyType::MinMachineIndex_UsedFirst},
        {"MinMachineIndex_UnusedFirst", PlacingStrategyType::MinMachineIndex_UnusedFirst},

        {"Tetris_NoReorder", PlacingStrategyType::Tetris_NoReorder},
        {"Tetris_UsedFirst", PlacingStrategyType::Tetris_UsedFirst},
        {"Tetris_UnusedFirst", PlacingStrategyType::Tetris_UnusedFirst},
};

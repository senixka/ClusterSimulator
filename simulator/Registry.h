#pragma once

#include <string>
#include <unordered_map>


enum class JobManagerType {
    RoundRobinBlockingHead = 0,
    RoundRobinNonBlockingHead = 1,
};

static const std::unordered_map<std::string, JobManagerType> nameToJobManagerType_{
        {"RoundRobinBlockingHead", JobManagerType::RoundRobinBlockingHead},
        {"RoundRobinNonBlockingHead", JobManagerType::RoundRobinNonBlockingHead},
};


enum class TaskManagerType {
    InJobOrder = 0,
};

static const std::unordered_map<std::string, TaskManagerType> nameToTaskManagerType_{
        {"InJobOrder", TaskManagerType::InJobOrder},
};


enum class PlacingStrategyType {
    Random = 0,
    MinVolume = 1,
    MaxVolume = 2,
    Tetris = 3,
};

static const std::unordered_map<std::string, PlacingStrategyType> nameToPlacingStrategyType_{
        {"Random", PlacingStrategyType::Random},
        {"MinVolume", PlacingStrategyType::MinVolume},
        {"MaxVolume", PlacingStrategyType::MaxVolume},
        {"Tetris", PlacingStrategyType::Tetris},
};

#pragma once

#include <string>
#include <unordered_map>


enum class JobManagerType {
    RoundRobin = 0,
};

static const std::unordered_map<std::string, JobManagerType> nameToJobManagerType_{
        {"RoundRobin", JobManagerType::RoundRobin},
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
    Tetris = 2,
};

static const std::unordered_map<std::string, PlacingStrategyType> nameToPlacingStrategyType_{
        {"Random", PlacingStrategyType::Random},
        {"MinVolume", PlacingStrategyType::MinVolume},
        {"Tetris", PlacingStrategyType::Tetris},
};

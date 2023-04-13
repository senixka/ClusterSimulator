#pragma once

#include "IPlacingStrategy.h"


enum PlacingStrategyType {
    Random = 0,
    MinVolume = 1,
    Tetris = 2,
};


class FactoryPlacingStrategy {
public:
    static IPlacingStrategy* Create(PlacingStrategyType placingStrategyType);
};

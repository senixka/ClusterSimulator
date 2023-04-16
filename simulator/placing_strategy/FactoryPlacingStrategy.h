#pragma once

#include "IPlacingStrategy.h"
#include "../Registry.h"


class FactoryPlacingStrategy {
public:
    static IPlacingStrategy* Create(PlacingStrategyType placingStrategyType);
};

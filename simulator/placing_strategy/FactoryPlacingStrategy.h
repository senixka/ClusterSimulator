#pragma once

#include "IPlacingStrategy.h"
#include "../Registry.h"


struct FactoryPlacingStrategy {
    static IPlacingStrategy* Create(PlacingStrategyType placingStrategyType);
};

#include "FactoryPlacingStrategy.h"

#include "Random.h"
#include "MinVolume.h"
#include "MaxVolume.h"
#include "Tetris.h"
#include "../Macro.h"


IPlacingStrategy* FactoryPlacingStrategy::Create(PlacingStrategyType placingStrategyType) {
    switch (placingStrategyType) {
        case PlacingStrategyType::Random:
            return (new (class Random){});

        case PlacingStrategyType::MinVolume:
            return (new (class MinVolume){});

        case PlacingStrategyType::MaxVolume:
            return (new (class MaxVolume){});

        case PlacingStrategyType::Tetris:
            return (new (class Tetris){});

        default:
            UNREACHABLE("FactoryPlacingStrategy");
    }
}

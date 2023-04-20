#include "FactoryPlacingStrategy.h"

#include "Random.h"
#include "MinVolume.h"
#include "MaxVolume.h"
#include "MinTaskCount.h"
#include "MaxTaskCount.h"
#include "MinMachineIndex.h"
#include "Tetris.h"

#include "priority_reorder/NoReorder.h"
#include "priority_reorder/UsedFirst.h"
#include "priority_reorder/UnusedFirst.h"

#include "../Macro.h"


IPlacingStrategy* FactoryPlacingStrategy::Create(PlacingStrategyType placingStrategyType) {
    switch (placingStrategyType) {
        case PlacingStrategyType::Random_NoReorder:
            return (new placing_strategy::Random<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::Random_UsedFirst:
            return (new placing_strategy::Random<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::Random_UnusedFirst:
            return (new placing_strategy::Random<placing_strategy::priority_reorder::UnusedFirst>{});


        case PlacingStrategyType::MinVolume_NoReorder:
            return (new placing_strategy::MinVolume<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::MinVolume_UsedFirst:
            return (new placing_strategy::MinVolume<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::MinVolume_UnusedFirst:
            return (new placing_strategy::MinVolume<placing_strategy::priority_reorder::UnusedFirst>{});


        case PlacingStrategyType::MaxVolume_NoReorder:
            return (new placing_strategy::MaxVolume<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::MaxVolume_UsedFirst:
            return (new placing_strategy::MaxVolume<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::MaxVolume_UnusedFirst:
            return (new placing_strategy::MaxVolume<placing_strategy::priority_reorder::UnusedFirst>{});


        case PlacingStrategyType::MinTaskCount_NoReorder:
            return (new placing_strategy::MinTaskCount<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::MinTaskCount_UsedFirst:
            return (new placing_strategy::MinTaskCount<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::MinTaskCount_UnusedFirst:
            return (new placing_strategy::MinTaskCount<placing_strategy::priority_reorder::UnusedFirst>{});


        case PlacingStrategyType::MaxTaskCount_NoReorder:
            return (new placing_strategy::MaxTaskCount<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::MaxTaskCount_UsedFirst:
            return (new placing_strategy::MaxTaskCount<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::MaxTaskCount_UnusedFirst:
            return (new placing_strategy::MaxTaskCount<placing_strategy::priority_reorder::UnusedFirst>{});


        case PlacingStrategyType::MinMachineIndex_NoReorder:
            return (new placing_strategy::MinMachineIndex<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::MinMachineIndex_UsedFirst:
            return (new placing_strategy::MinMachineIndex<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::MinMachineIndex_UnusedFirst:
            return (new placing_strategy::MinMachineIndex<placing_strategy::priority_reorder::UnusedFirst>{});


        case PlacingStrategyType::Tetris_NoReorder:
            return (new placing_strategy::Tetris<placing_strategy::priority_reorder::NoReorder>{});

        case PlacingStrategyType::Tetris_UsedFirst:
            return (new placing_strategy::Tetris<placing_strategy::priority_reorder::UsedFirst>{});

        case PlacingStrategyType::Tetris_UnusedFirst:
            return (new placing_strategy::Tetris<placing_strategy::priority_reorder::UnusedFirst>{});


        default:
            UNREACHABLE("FactoryPlacingStrategy");
    }
}

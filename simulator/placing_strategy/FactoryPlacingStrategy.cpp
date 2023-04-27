#include "FactoryPlacingStrategy.h"

#include "Random.h"
#include "MinVolume.h"
#include "MaxVolume.h"
#include "MinTaskCount.h"
#include "MaxTaskCount.h"
#include "MinMachineIndex.h"
#include "Tetris.h"

#include "partition_policy/NoPartition.h"
#include "partition_policy/UsedFirst.h"
#include "partition_policy/UnusedFirst.h"

#include "../Macro.h"


IPlacingStrategy* FactoryPlacingStrategy::Create(PlacingStrategyType placingStrategyType) {
    switch (placingStrategyType) {
        case PlacingStrategyType::Random_NoPartition:
            return (new placing_strategy::Random<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::Random_UsedFirst:
            return (new placing_strategy::Random<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::Random_UnusedFirst:
            return (new placing_strategy::Random<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinVolume_NoPartition:
            return (new placing_strategy::MinVolume<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinVolume_UsedFirst:
            return (new placing_strategy::MinVolume<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinVolume_UnusedFirst:
            return (new placing_strategy::MinVolume<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxVolume_NoPartition:
            return (new placing_strategy::MaxVolume<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxVolume_UsedFirst:
            return (new placing_strategy::MaxVolume<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxVolume_UnusedFirst:
            return (new placing_strategy::MaxVolume<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinTaskCount_NoPartition:
            return (new placing_strategy::MinTaskCount<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinTaskCount_UsedFirst:
            return (new placing_strategy::MinTaskCount<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinTaskCount_UnusedFirst:
            return (new placing_strategy::MinTaskCount<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxTaskCount_NoPartition:
            return (new placing_strategy::MaxTaskCount<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxTaskCount_UsedFirst:
            return (new placing_strategy::MaxTaskCount<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxTaskCount_UnusedFirst:
            return (new placing_strategy::MaxTaskCount<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinMachineIndex_NoPartition:
            return (new placing_strategy::MinMachineIndex<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinMachineIndex_UsedFirst:
            return (new placing_strategy::MinMachineIndex<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinMachineIndex_UnusedFirst:
            return (new placing_strategy::MinMachineIndex<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::Tetris_NoPartition:
            return (new placing_strategy::Tetris<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::Tetris_UsedFirst:
            return (new placing_strategy::Tetris<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::Tetris_UnusedFirst:
            return (new placing_strategy::Tetris<placing_strategy::partition_policy::UnusedFirst>{});


        default:
            UNREACHABLE("FactoryPlacingStrategy");
    }
}

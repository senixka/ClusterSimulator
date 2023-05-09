#include "FactoryPlacingStrategy.h"

#include "Tetris.h"
#include "Random.h"
#include "MinCPU.h"
#include "MaxCPU.h"
#include "MinSum.h"
#include "MaxSum.h"
#include "MinMemory.h"
#include "MaxMemory.h"
#include "MinTaskCount.h"
#include "MaxTaskCount.h"
#include "MinVolumeBefore.h"
#include "MaxVolumeBefore.h"
#include "MinVolumeAfter.h"
#include "MaxVolumeAfter.h"
#include "MinMachineIndex.h"


#include "partition_policy/NoPartition.h"
#include "partition_policy/UsedFirst.h"
#include "partition_policy/UnusedFirst.h"

#include "../Macro.h"


IPlacingStrategy* FactoryPlacingStrategy::Create(PlacingStrategyType placingStrategyType) {
    switch (placingStrategyType) {

        case PlacingStrategyType::Tetris_NoPartition:
            return (new placing_strategy::Tetris<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::Tetris_UsedFirst:
            return (new placing_strategy::Tetris<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::Tetris_UnusedFirst:
            return (new placing_strategy::Tetris<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinCPU_NoPartition:
            return (new placing_strategy::MinCPU<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinCPU_UsedFirst:
            return (new placing_strategy::MinCPU<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinCPU_UnusedFirst:
            return (new placing_strategy::MinCPU<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxCPU_NoPartition:
            return (new placing_strategy::MaxCPU<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxCPU_UsedFirst:
            return (new placing_strategy::MaxCPU<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxCPU_UnusedFirst:
            return (new placing_strategy::MaxCPU<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinMemory_NoPartition:
            return (new placing_strategy::MinMemory<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinMemory_UsedFirst:
            return (new placing_strategy::MinMemory<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinMemory_UnusedFirst:
            return (new placing_strategy::MinMemory<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxMemory_NoPartition:
            return (new placing_strategy::MaxMemory<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxMemory_UsedFirst:
            return (new placing_strategy::MaxMemory<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxMemory_UnusedFirst:
            return (new placing_strategy::MaxMemory<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinSum_NoPartition:
            return (new placing_strategy::MinSum<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinSum_UsedFirst:
            return (new placing_strategy::MinSum<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinSum_UnusedFirst:
            return (new placing_strategy::MinSum<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxSum_NoPartition:
            return (new placing_strategy::MaxSum<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxSum_UsedFirst:
            return (new placing_strategy::MaxSum<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxSum_UnusedFirst:
            return (new placing_strategy::MaxSum<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::Random_NoPartition:
            return (new placing_strategy::Random<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::Random_UsedFirst:
            return (new placing_strategy::Random<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::Random_UnusedFirst:
            return (new placing_strategy::Random<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinVolumeBefore_NoPartition:
            return (new placing_strategy::MinVolumeBefore<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinVolumeBefore_UsedFirst:
            return (new placing_strategy::MinVolumeBefore<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinVolumeBefore_UnusedFirst:
            return (new placing_strategy::MinVolumeBefore<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxVolumeBefore_NoPartition:
            return (new placing_strategy::MaxVolumeBefore<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxVolumeBefore_UsedFirst:
            return (new placing_strategy::MaxVolumeBefore<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxVolumeBefore_UnusedFirst:
            return (new placing_strategy::MaxVolumeBefore<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MinVolumeAfter_NoPartition:
            return (new placing_strategy::MinVolumeAfter<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MinVolumeAfter_UsedFirst:
            return (new placing_strategy::MinVolumeAfter<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MinVolumeAfter_UnusedFirst:
            return (new placing_strategy::MinVolumeAfter<placing_strategy::partition_policy::UnusedFirst>{});


        case PlacingStrategyType::MaxVolumeAfter_NoPartition:
            return (new placing_strategy::MaxVolumeAfter<placing_strategy::partition_policy::NoPartition>{});

        case PlacingStrategyType::MaxVolumeAfter_UsedFirst:
            return (new placing_strategy::MaxVolumeAfter<placing_strategy::partition_policy::UsedFirst>{});

        case PlacingStrategyType::MaxVolumeAfter_UnusedFirst:
            return (new placing_strategy::MaxVolumeAfter<placing_strategy::partition_policy::UnusedFirst>{});


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


        default:
            UNREACHABLE("FactoryPlacingStrategy");
    }
}

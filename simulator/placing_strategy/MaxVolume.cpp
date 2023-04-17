#include "MaxVolume.h"

#include "../Macro.h"


size_t MaxVolume::BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* /*task*/) {
    ASSERT(!machines.empty());

    unsigned bestMachineIndex{0};
    double maxVolume{-10};

    for (const auto& [machineResources, machineIndex] : machines) {
        const auto mVolume = machineResources.get<0>() * machineResources.get<1>() * machineResources.get<2>();

        if (mVolume > maxVolume) {
            maxVolume = mVolume;
            bestMachineIndex = machineIndex;
        }
    }

    return bestMachineIndex;
}

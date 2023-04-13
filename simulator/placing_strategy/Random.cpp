#include "Random.h"

#include "../Macro.h"

#include <random>


size_t Random::BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* /*task*/) {
    ASSERT(!machines.empty());

    return machines[std::rand() % machines.size()].second;
}

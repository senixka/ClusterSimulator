#include "MachineManager.h"

#include "Macro.h"

#include <fstream>
#include <limits>


MachineManager::MachineManager(const std::string& inputFilePath) {
    printf("Start of MachineManager construction\n");

    std::ifstream in;
    in.open(inputFilePath);

    size_t totalMachine, nConfig;
    in >> totalMachine >> nConfig;

    ASSERT(totalMachine < UINT32_MAX);

    machines_.reserve(totalMachine);

    unsigned machineIndex{0};
    for (size_t i = 0; i < nConfig; ++i) {
        uint64_t nMachine, machineCpu, machineMemory;
        in >> nMachine >> machineCpu >> machineMemory;

        ASSERT(nMachine < UINT32_MAX);
        ASSERT(machineCpu < std::numeric_limits<ResourceT>::max());
        ASSERT(machineMemory < std::numeric_limits<ResourceT>::max());

        for (uint64_t j = 0; j < nMachine; ++j) {
            machines_.push_back(
                Machine(machineIndex++, static_cast<ResourceT>(machineCpu), static_cast<ResourceT>(machineMemory))
            );
        }
    }
    ASSERT(machineIndex == totalMachine);

    std::vector<tree_entry> entries;
    entries.reserve(machines_.size());

    for (const Machine& m : machines_) {
        entries.push_back({{m.availableCpu_, m.availableMemory_}, m.machineIndex_});
    }

    // https://groups.google.com/g/boost-list/c/pPgWc2Wf2Bo
    tree_ = std::move(rtree_2d(entries.begin(), entries.end()));

    printf("End of MachineManager construction\n");
}

void MachineManager::FindSuitableMachines(const Task& task, std::vector<const Machine*>& result) {
    box_2d query_box({task.cpuRequest_, task.memoryRequest_}, {MACHINE_MAX_POSSIBLE_CPU, MACHINE_MAX_POSSIBLE_MEMORY});

    innerQueryResults_.clear();
    tree_.query(bgi::intersects(query_box), std::back_inserter(innerQueryResults_));

    result.clear();
    for (const auto& it : innerQueryResults_) {
        result.push_back(&machines_[it.second]);
    }
}

void MachineManager::PlaceTaskOnMachine(const Task& task) {
    Machine& m = machines_[task.machineIndex_];

    tree_.remove({{m.availableCpu_, m.availableMemory_}, m.machineIndex_});
    m.PlaceTask(task);
    tree_.insert({{m.availableCpu_, m.availableMemory_}, m.machineIndex_});
}

void MachineManager::RemoveTaskFromMachine(const Task& task) {
    Machine& m = machines_[task.machineIndex_];

    tree_.remove({{m.availableCpu_, m.availableMemory_}, m.machineIndex_});
    m.RemoveTask(task);
    tree_.insert({{m.availableCpu_, m.availableMemory_}, m.machineIndex_});
}

const std::vector<Machine>& MachineManager::GetAllMachines() const {
    return machines_;
}

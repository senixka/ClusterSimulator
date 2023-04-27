#include "MachineManager.h"

#include "Macro.h"
#include "Defines.h"

#include <fstream>


MachineManager::MachineManager(const std::string& inputFilePath) {
    printf("Start of MachineManager construction\n");

    std::ifstream in;
    in.open(inputFilePath);

    size_t nConfig;
    in >> nConfig;

    size_t nMachine;
    unsigned machineIndex{0}, machineCpu, machineMemory;

    for (size_t i = 0; i < nConfig; ++i) {
        in >> machineCpu >> machineMemory >> nMachine;

        for (size_t j = 0; j < nMachine; ++j) {
            machines_.emplace_back(Machine(machineIndex++, 0, machineCpu, machineMemory));
        }
    }

    std::vector<tree_entry> entries;
    entries.reserve(machines_.size());

    for (const auto& m : machines_) {
        entries.push_back({{m.availableCpu_, m.availableMemory_}, m.machineIndex_});
    }

    printf("In MachineManager, start of rtree construction\n");
    tree_ = std::move(rtree_2d(entries.begin(), entries.end()));
    printf("In MachineManager, end of rtree construction\n");

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

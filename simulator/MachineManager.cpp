#include "MachineManager.h"

#include "Macro.h"

#include <fstream>


MachineManager::MachineManager(const std::string& inputFilePath) {
    std::ifstream in;
    in.open(inputFilePath);

    size_t nConfig;
    in >> nConfig;

    size_t nMachine;
    double machineCpu, machineMemory, machineDisk;
    unsigned machineIndex{0};

    for (size_t i = 0; i < nConfig; ++i) {
        in >> machineCpu >> machineMemory >> machineDisk >> nMachine;

        for (size_t j = 0; j < nMachine; ++j) {
            machines_.push_back({machineIndex, 0, machineCpu, machineMemory, machineDisk});

            point_3d point(machineCpu, machineMemory, machineDisk);
            tree_.insert({point, machineIndex++});

            ASSERT(machines_.back().machineIndex_ == machineIndex - 1);
        }
    }
}

void MachineManager::FindSuitableMachines(const Task& task, std::vector<const Machine*>& result) {
    box_3d query_box({task.cpuRequest_, task.memoryRequest_, task.diskSpaceRequest_}, {1, 1, 1});

    innerQueryResults_.clear();
    tree_.query(bgi::intersects(query_box), std::back_inserter(innerQueryResults_));

    result.clear();
    for (const auto& it : innerQueryResults_) {
        result.push_back(&machines_[it.second]);
    }
}

void MachineManager::PlaceTaskOnMachine(const Task& task) {
    auto [oldCpu, oldMemory, oldDisk] = machines_[task.machineIndex_].GetResources();
    tree_.remove({{oldCpu, oldMemory, oldDisk}, task.machineIndex_});

    machines_[task.machineIndex_].PlaceTask(task);

    auto [newCpu, newMemory, newDisk] = machines_[task.machineIndex_].GetResources();
    tree_.insert({{newCpu, newMemory, newDisk}, task.machineIndex_});
}

void MachineManager::RemoveTaskFromMachine(const Task& task) {
    auto [oldCpu, oldMemory, oldDisk] = machines_[task.machineIndex_].GetResources();
    tree_.remove({{oldCpu, oldMemory, oldDisk}, task.machineIndex_});

    machines_[task.machineIndex_].RemoveTask(task);

    auto [newCpu, newMemory, newDisk] = machines_[task.machineIndex_].GetResources();
    tree_.insert({{newCpu, newMemory, newDisk}, task.machineIndex_});
}

const std::vector<Machine>& MachineManager::GetAllMachines() const {
    return machines_;
}

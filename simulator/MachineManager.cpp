#include "MachineManager.h"

#include <fstream>


MachineManager::MachineManager(const std::string& inputFilePath) {
    std::ifstream in;
    in.open(inputFilePath);

    size_t nConfig;
    in >> nConfig;

    size_t nMachine;
    double machineCpu, machineMemory, machineDisk;

    for (size_t i = 0; i < nConfig; ++i) {
        in >> machineCpu >> machineMemory >> machineDisk >> nMachine;

        for (size_t j = 0; j < nMachine; ++j) {
            machines_.push_back({machineCpu, machineMemory, machineDisk});

            point_3d point(machineCpu, machineMemory, machineDisk);
            tree_.insert({point, machines_.size() - 1});
        }
    }
}

void MachineManager::FindSuitableMachines(const Task& task, ReturnQueryType& result) {
    box_3d query_box({task.cpuRequest_, task.memoryRequest_, task.diskSpaceRequest_}, {1, 1, 1});

    tree_.query(bgi::intersects(query_box), std::back_inserter(result));
}

void MachineManager::PlaceTaskOnMachine(const Task& task) {
    auto [oldCpu, oldMemory, oldDisk] = machines_[task.machineIndex_];
    tree_.remove({{oldCpu, oldMemory, oldDisk}, task.machineIndex_});

    machines_[task.machineIndex_].PlaceTask(task);

    auto [newCpu, newMemory, newDisk] = machines_[task.machineIndex_];
    tree_.insert({{newCpu, newMemory, newDisk}, task.machineIndex_});
}

void MachineManager::RemoveTaskFromMachine(const Task& task) {
    auto [oldCpu, oldMemory, oldDisk] = machines_[task.machineIndex_];
    tree_.remove({{oldCpu, oldMemory, oldDisk}, task.machineIndex_});

    machines_[task.machineIndex_].RemoveTask(task);

    auto [newCpu, newMemory, newDisk] = machines_[task.machineIndex_];
    tree_.insert({{newCpu, newMemory, newDisk}, task.machineIndex_});
}

const std::vector<Machine>& MachineManager::GetAllMachines() const {
    return machines_;
}

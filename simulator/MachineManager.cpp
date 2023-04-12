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
            machines.push_back({machineCpu, machineMemory, machineDisk});

            point_3d point(machineCpu, machineMemory, machineDisk);
            tree.insert({point, machines.size() - 1});
        }
    }
}

void MachineManager::FindSuitableMachines(const Task& task, ReturnQueryType& result) {
    box_3d query_box({task.cpuRequest, task.memoryRequest, task.diskSpaceRequest}, {1, 1, 1});

    tree.query(bgi::intersects(query_box), std::back_inserter(result));
}

void MachineManager::PlaceTaskOnMachine(const Task& task) {
    auto [old_cpu, old_memory, old_disk] = machines[task.machineIndex];
    tree.remove({{old_cpu, old_memory, old_disk}, task.machineIndex});

    machines[task.machineIndex].PlaceTask(task);

    auto [new_cpu, new_memory, new_disk] = machines[task.machineIndex];
    tree.insert({{new_cpu, new_memory, new_disk}, task.machineIndex});
}

void MachineManager::RemoveTaskFromMachine(const Task& task) {
    auto [old_cpu, old_memory, old_disk] = machines[task.machineIndex];
    tree.remove({{old_cpu, old_memory, old_disk}, task.machineIndex});

    machines[task.machineIndex].RemoveTask(task);

    auto [new_cpu, new_memory, new_disk] = machines[task.machineIndex];
    tree.insert({{new_cpu, new_memory, new_disk}, task.machineIndex});
}

const std::vector<Machine>& MachineManager::GetAllMachines() {
    return machines;
}

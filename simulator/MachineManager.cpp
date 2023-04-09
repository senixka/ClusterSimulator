#include "MachineManager.h"

#include <fstream>


MachineManager::MachineManager(const std::string& inputFilePath) {
    std::ifstream in;
    in.open(inputFilePath);

    size_t nMachine;
    in >> nMachine;

    machines.resize(nMachine);

    for (size_t i = 0; i < nMachine; ++i) {
        in >> machines[i].cpuCapacity >> machines[i].memoryCapacity >> machines[i].diskSpaceCapacity;

        point_3d point(machines[i].cpuCapacity, machines[i].memoryCapacity, machines[i].diskSpaceCapacity);
        tree.insert({point, i});
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

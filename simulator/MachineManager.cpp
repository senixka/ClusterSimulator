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

void MachineManager::PlaceTaskOnMachine(Task& task, size_t machineIndex) {
    auto [old_cpu, old_memory, old_disk] = machines[machineIndex];
    tree.remove({{old_cpu, old_memory, old_disk}, machineIndex});

    machines[machineIndex].PlaceTask(task);

    auto [new_cpu, new_memory, new_disk] = machines[machineIndex];
    tree.insert({{new_cpu, new_memory, new_disk}, machineIndex});
}

void MachineManager::RemoveTaskFromMachine(const Task& task, size_t machineIndex) {
    auto [old_cpu, old_memory, old_disk] = machines[machineIndex];
    tree.remove({{old_cpu, old_memory, old_disk}, machineIndex});

    machines[machineIndex].RemoveTask(task);

    auto [new_cpu, new_memory, new_disk] = machines[machineIndex];
    tree.insert({{new_cpu, new_memory, new_disk}, machineIndex});
}

const std::vector<Machine>& MachineManager::GetAllMachines() {
    return machines;
}

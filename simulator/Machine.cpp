#include "Machine.h"

#include <cassert>


bool Machine::IsTaskPlaceable(const Task& task) const {
    return task.cpuRequest <= currentCpu
           && task.memoryRequest <= currentMemory
           && task.diskSpaceRequest <= currentDiskSpace;
}

void Machine::PlaceTask(const Task& task) {
    assert(IsTaskPlaceable(task));

    currentCpu -= task.cpuRequest;
    currentMemory -= task.memoryRequest;
    currentDiskSpace -= task.diskSpaceRequest;
}

void Machine::RemoveTask(const Task& task) {
    currentCpu += task.cpuRequest;
    currentMemory += task.memoryRequest;
    currentDiskSpace += task.diskSpaceRequest;
}

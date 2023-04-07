#include "Machine.h"

#include <cassert>


bool Machine::IsTaskPlaceable(const Task& task) const {
    return task.cpuRequest <= cpuCapacity
           && task.memoryRequest <= memoryCapacity
           && task.diskSpaceRequest <= diskSpaceCapacity;
}

void Machine::PlaceTask(const Task& task) {
    assert(IsTaskPlaceable(task));

    cpuCapacity -= task.cpuRequest;
    memoryCapacity -= task.memoryRequest;
    diskSpaceCapacity -= task.diskSpaceRequest;
}

void Machine::RemoveTask(const Task& task) {
    cpuCapacity += task.cpuRequest;
    memoryCapacity += task.memoryRequest;
    diskSpaceCapacity += task.diskSpaceRequest;
}

#include "Machine.h"

#include "Macro.h"


bool Machine::IsTaskPlaceable(const Task& task) const {
    return task.cpuRequest_ <= currentCpu_
           && task.memoryRequest_ <= currentMemory_
           && task.diskSpaceRequest_ <= currentDiskSpace_;
}

void Machine::PlaceTask(const Task& task) {
    ASSERT(IsTaskPlaceable(task));

    currentCpu_ -= task.cpuRequest_;
    currentMemory_ -= task.memoryRequest_;
    currentDiskSpace_ -= task.diskSpaceRequest_;
}

void Machine::RemoveTask(const Task& task) {
    currentCpu_ += task.cpuRequest_;
    currentMemory_ += task.memoryRequest_;
    currentDiskSpace_ += task.diskSpaceRequest_;
}

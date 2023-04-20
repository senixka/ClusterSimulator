#include "Machine.h"

#include "Macro.h"


//Machine::Machine(size_t machineIndex, size_t currentTaskCount,
//                 double currentCpu, double currentMemory, double currentDiskSpace)
//                 : machineIndex_(machineIndex), currentTaskCount_(currentTaskCount),
//                   currentCpu_(currentCpu), currentMemory_(currentMemory), currentDiskSpace_(currentDiskSpace) {
//}

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

    ++currentTaskCount_;
}

void Machine::RemoveTask(const Task& task) {
    ASSERT(currentTaskCount_ > 0);

    currentCpu_ += task.cpuRequest_;
    currentMemory_ += task.memoryRequest_;
    currentDiskSpace_ += task.diskSpaceRequest_;

    --currentTaskCount_;
}

std::tuple<double, double, double> Machine::GetResources() const {
    return {currentCpu_, currentMemory_, currentDiskSpace_};
}

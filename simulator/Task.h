#pragma once

#include "ClusterEvent.h"
#include "Defines.h"
#include "Macro.h"


class Task : public ClusterEvent {
public:
    explicit Task(uint64_t estimate, unsigned cpuRequest, unsigned memoryRequest, unsigned jobID)
             : estimate_(estimate), cpuRequest_(cpuRequest), memoryRequest_(memoryRequest), jobID_(jobID) {
        ASSERT(1 <= cpuRequest_);
        ASSERT(cpuRequest_ <= MACHINE_MAX_POSSIBLE_CPU);

        ASSERT(1 <= memoryRequest_);
        ASSERT(memoryRequest_ <= MACHINE_MAX_POSSIBLE_MEMORY);
    }

public:
    const uint64_t estimate_{0};
    const unsigned cpuRequest_{0};
    const unsigned memoryRequest_{0};
    const unsigned jobID_{0};
    unsigned machineIndex_{UINT32_MAX};
};

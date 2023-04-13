#pragma once

#include "ClusterEvent.h"


class Task : public ClusterEvent {
public:
    explicit Task(uint64_t jobID, uint64_t estimate, double cpuRequest,
                  double memoryRequest, double diskSpaceRequest, unsigned taskIndex)
      : jobID_(jobID), estimate_(estimate), cpuRequest_(cpuRequest),
        memoryRequest_(memoryRequest), diskSpaceRequest_(diskSpaceRequest), taskIndex_(taskIndex) {
    }

public:
    const uint64_t jobID_{0};
    const uint64_t estimate_{0};
    const double cpuRequest_{0};
    const double memoryRequest_{0};
    const double diskSpaceRequest_{0};
    const unsigned taskIndex_{0};
    unsigned machineIndex_{UINT32_MAX};
};

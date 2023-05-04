#pragma once

#include "ClusterEvent.h"
#include "Defines.h"
#include "Macro.h"

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>


class Task : public ClusterEvent {
public:
    Task() = default;

    static void PrepareMemoryPool(unsigned nTask);
    static Task* New(BoundedTimeT estimate, ResourceT cpuRequest, ResourceT memoryRequest, JobIdT jobId);
    static void Delete(Task* ptr);

public:
    BoundedTimeT estimate_{0};
    ResourceT cpuRequest_{0};
    ResourceT memoryRequest_{0};
    JobIdT jobId_{0};
    unsigned machineIndex_{UINT32_MAX};
};

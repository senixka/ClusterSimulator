#ifndef CLUSTERSIMULATOR_TASK_H
#define CLUSTERSIMULATOR_TASK_H

#include "ClusterEvent.h"


class Job;

struct Task : public ClusterEvent {
    uint64_t estimate{0};
    Job* job{nullptr};
    double cpuRequest{0};
    double memoryRequest{0};
    double diskSpaceRequest{0};
    unsigned machineIndex{UINT32_MAX};
};


#endif //CLUSTERSIMULATOR_TASK_H

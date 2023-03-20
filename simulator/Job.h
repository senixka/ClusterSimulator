#ifndef CLUSTERSIMULATOR_JOB_H
#define CLUSTERSIMULATOR_JOB_H

#include "Task.h"
#include "ClusterEvent.h"

#include <list>
#include <string>
#include <iostream>


class Job : public ClusterEvent {
public:
    uint64_t jobTime{0};
    uint64_t jobID{0};
    std::string user;
    std::list<Task> pendingTask;

    Job() = default;
    Job(std::istream& in);
};


#endif //CLUSTERSIMULATOR_JOB_H

#pragma once

class Cluster;


class IScheduler {
public:
    virtual void OnJobSubmitted(Cluster& cluster) = 0;
    virtual void OnTaskFinished(Cluster& cluster) = 0;
    virtual void Schedule(Cluster& cluster) = 0;

    virtual ~IScheduler() = default;
};

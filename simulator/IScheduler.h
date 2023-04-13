#ifndef CLUSTERSIMULATOR_ISCHEDULER_H
#define CLUSTERSIMULATOR_ISCHEDULER_H

class Cluster;


class IScheduler {
public:
    virtual void OnJobSubmitted(Cluster& cluster) = 0;
    virtual void OnTaskFinished(Cluster& cluster) = 0;
    virtual void Schedule(Cluster& cluster) = 0;

    virtual ~IScheduler() = default;
};


#endif //CLUSTERSIMULATOR_ISCHEDULER_H

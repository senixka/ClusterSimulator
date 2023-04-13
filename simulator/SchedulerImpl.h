#pragma once

#include "IScheduler.h"
#include "placing_strategy/IPlacingStrategy.h"


class SchedulerImpl : public IScheduler {
public:
    explicit SchedulerImpl(IPlacingStrategy* currentPlacingStrategy);

    void OnJobSubmitted(Cluster& cluster) override;
    void OnTaskFinished(Cluster& cluster) override;
    void Schedule(Cluster& cluster) override;

private:
    IPlacingStrategy* placingStrategy{nullptr};
};

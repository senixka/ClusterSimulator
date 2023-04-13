#pragma once

#include "IScheduler.h"
#include "placing_strategy/IPlacingStrategy.h"

#include <memory>


class SchedulerImpl : public IScheduler {
public:
    explicit SchedulerImpl(std::shared_ptr<IPlacingStrategy> placingStrategy);

    void OnJobSubmitted(Cluster& cluster) override;
    void OnTaskFinished(Cluster& cluster) override;
    void Schedule(Cluster& cluster) override;

private:
    std::shared_ptr<IPlacingStrategy> placingStrategy_{nullptr};
};

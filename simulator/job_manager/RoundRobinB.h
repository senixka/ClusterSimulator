#pragma once

#include "IJobManager.h"
#include "../Job.h"

#include <list>


namespace job_manager::detail {

template<unsigned kRounds>
class RoundRobinB : public IJobManager {
public:
    void PutJob(Job* job) override {
        jobs_.push_back(job);
    }

    Job* GetJob() override {
        ASSERT(it_ != jobs_.end());
        return *it_;
    }

    void ReturnJob(Job* job, bool isModified) override {
        if (job->taskManager_->TaskCount() == 0) {
            it_ = jobs_.erase(it_);
            delete job;
        } else if (isModified) {
            ++it_;
        }

        if (it_ == jobs_.end()) {
            it_ = jobs_.begin();
            ++currentRound_;
        }
    }

    size_t JobCount() override {
        return jobs_.size();
    }

    void NewSchedulingCycle() override {
        it_ = jobs_.begin();
        currentRound_ = 0;
    }

    bool IsThereSomethingElse() override {
        return !jobs_.empty() && currentRound_ < kRounds;
    }

    ~RoundRobinB() {
        for (Job* job : jobs_) {
            delete job;
        }
    }

private:
    unsigned currentRound_{0};
    std::list<Job*> jobs_;
    typename std::list<Job*>::iterator it_;
};

} // namespace job_manager::detail


namespace job_manager {

using RoundRobinB_1 = class detail::RoundRobinB<1u>;
using RoundRobinB_10 = class detail::RoundRobinB<10u>;
using RoundRobinB_100 = class detail::RoundRobinB<100u>;

} // namespace job_manager

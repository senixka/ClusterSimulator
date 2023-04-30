#pragma once

#include "IJobManager.h"
#include "../Job.h"

#include <list>


namespace job_manager::detail {

template<unsigned kRounds>
class RoundRobinNB : public IJobManager {
public:
    void PutJob(Job* job) override {
        jobs_.push_back(job);
    }

    Job* GetJob() override {
        ASSERT(it_ != jobs_.end());
        return *it_;
    }

    void ReturnJob(Job* job, bool /*isModified*/) override {
        if (job->taskManager_->TaskCount() == 0) {
            it_ = jobs_.erase(it_);
            delete job;
        } else {
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

    ~RoundRobinNB() {
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

using RoundRobinNB_1 = class detail::RoundRobinNB<1u>;
using RoundRobinNB_10 = class detail::RoundRobinNB<10u>;
using RoundRobinNB_100 = class detail::RoundRobinNB<100u>;

} // namespace job_manager

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
        it_ = jobs_.begin();
    }

    Job* GetJob() override {
        ASSERT(it_ != jobs_.end());
        ASSERT((*it_)->taskManager_->IsThereSomethingElse());

        return *it_;
    }

    void ReturnJob(Job* job, bool /*isModified*/) override {
        ++currentIter_;

        if (job->taskManager_->TaskCount() == 0) {
            it_ = jobs_.erase(it_);

            ASSERT(currentSize_ >= currentIter_);
            currentIter_ += (currentSize_ - currentIter_) / currentTaskCount_;

            Job::Delete(job);
        } else {
            ++it_;
        }

        FindNextIt();
    }

    void NewSchedulingCycle() override {
        for (Job* job : jobs_) {
            job->taskManager_->NewSchedulingCycle();
        }

        isNextIterValid = true;
        currentIter_ = 0;
        currentTaskCount_ = jobs_.size();
        currentSize_ = currentTaskCount_ * kRounds;

        FindNextIt();
    }

    bool IsThereSomethingElse() override {
        return currentIter_ < currentSize_ && isNextIterValid;
    }

    ~RoundRobinNB() {
        for (Job* job : jobs_) {
            Job::Delete(job);
        }
    }

private:
    void FindNextIt() {
        const size_t size = jobs_.size();
        if (size == 0) {
            isNextIterValid = false;
            return;
        }

        for (size_t i = 0; i < size; ++i) {
            if (it_ == jobs_.end()) {
                it_ = jobs_.begin();
            }

            if (!(*it_)->taskManager_->IsThereSomethingElse()) {
                ++it_;
                ++currentIter_;
            } else {
                return;
            }
        }

        isNextIterValid = false;
    }


private:
    size_t currentIter_{0};
    size_t currentSize_{0};
    size_t currentTaskCount_{0};
    bool isNextIterValid{true};

    std::list<Job*> jobs_;
    typename std::list<Job*>::iterator it_{jobs_.begin()};
};

} // namespace job_manager::detail


namespace job_manager {

using RoundRobinNB_1 = class detail::RoundRobinNB<1u>;
using RoundRobinNB_10 = class detail::RoundRobinNB<10u>;
using RoundRobinNB_100 = class detail::RoundRobinNB<100u>;

} // namespace job_manager

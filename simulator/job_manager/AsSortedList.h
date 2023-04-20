#pragma once

#include "IJobManager.h"
#include "../Macro.h"

#include <list>


namespace job_manager::detail {

template<class ListPtrCmp>
class AsSortedList : public IJobManager {
public:
    void PutJob(Job* job) override {
        jobs_.push_back(job);

        it_ = jobs_.begin();
    }

    Job* GetJob() override {
        ASSERT(!jobs_.empty());

        if (it_ == jobs_.end()) {
            it_ = jobs_.begin();
        }

        Job* job = *it_;
        it_ = jobs_.erase(it_);
        return job;
    }

    void ReturnJob(Job* job, bool /*isModified*/) override {
        if (job->taskManager_->TaskCount() == 0) {
            delete job;
            return;
        }

        jobs_.insert(it_, job);
    }

    size_t JobCount() override {
        return jobs_.size();
    }

    void NewSchedulingCycle() override {
        jobs_.sort(cmp_);
        it_ = jobs_.begin();
    }

    ~AsSortedList() {
        for (Job* job : jobs_) {
            delete job;
        }
    }

private:
    ListPtrCmp cmp_;

    std::list<Job*> jobs_;
    typename std::list<Job*>::iterator it_;
};

} // namespace job_manager::detail

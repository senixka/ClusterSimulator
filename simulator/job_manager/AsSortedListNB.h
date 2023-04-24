#pragma once

#include "IJobManager.h"
#include "../Macro.h"

#include <list>
#include <vector>
#include <algorithm>


namespace job_manager::detail {

template<class ComparePolicy>
class AsSortedListNB : public IJobManager {
public:
    void PutJob(Job* job) override {
        modified_.push_back(job);
    }

    Job* GetJob() override {
        ASSERT(JobCount() > 0);

        if (it_ == jobs_.end()) {
            NewSchedulingCycle();
        }

        Job* job = *it_;
        it_ = jobs_.erase(it_);
        return job;
    }

    void ReturnJob(Job* job, bool isModified) override {
        if (isModified) {
            if (job->taskManager_->TaskCount() == 0) {
                delete job;
                return;
            }

            modified_.push_back(job);
        } else {
            jobs_.insert(it_, job);
        }
    }

    size_t JobCount() override {
        return jobs_.size() + modified_.size();
    }

    void NewSchedulingCycle() override {
        MergeJobs();
        it_ = jobs_.begin();
    }

    ~AsSortedListNB() {
        for (Job* job : jobs_) {
            delete job;
        }

        for (Job* job : modified_){
            delete job;
        }
    }

private:
    void MergeJobs() {
        const size_t beforeSize = JobCount();

        std::sort(modified_.begin(), modified_.end(), ComparePolicy::Compare);

        auto j = jobs_.begin();
        auto m = modified_.begin();

        while (j != jobs_.end() && m != modified_.end()) {
            if (ComparePolicy::Compare(*m, *j)) {
                jobs_.insert(j, *m);
                ++m;
            } else {
                ++j;
            }
        }

        jobs_.insert(jobs_.end(), m, modified_.end());
        modified_.clear();

        ASSERT(beforeSize == JobCount());
        ASSERT(std::is_sorted(jobs_.begin(), jobs_.end(), ComparePolicy::Compare));
    }

private:
    std::list<Job*> jobs_;
    std::vector<Job*> modified_;

    typename std::list<Job*>::iterator it_;
};

} // namespace job_manager::detail

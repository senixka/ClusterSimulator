#pragma once

#include "IJobManager.h"
#include "../Macro.h"

#include <list>
#include <vector>
#include <algorithm>


namespace job_manager::detail {

template<class ComparePolicy, unsigned kIterPerJob>
class AsSortedListNB : public IJobManager {
public:
    void PutJob(Job* job) override {
        modified_.push_back(job);
    }

    Job* GetJob() override {
        ASSERT(it_ != jobs_.end());
        return *it_;
    }

    void ReturnJob(Job* job, bool isModified) override {
        if (job->taskManager_->TaskCount() == 0) {
            it_ = jobs_.erase(it_);
            isCurrentJobModified_ = false;
            currentJobIter_ = 0;

            delete job;
            return;
        }

        isCurrentJobModified_ |= isModified;

        if (++currentJobIter_ >= kIterPerJob) {
            if (isCurrentJobModified_) {
                modified_.push_back(job);
                it_ = jobs_.erase(it_);
            } else {
                ++it_;
            }

            isCurrentJobModified_ = false;
            currentJobIter_ = 0;
        }
    }

    size_t JobCount() override {
        return jobs_.size() + modified_.size();
    }

    void NewSchedulingCycle() override {
        MergeJobs();

        it_ = jobs_.begin();
        isCurrentJobModified_ = false;
        currentJobIter_ = 0;
    }

    bool IsThereSomethingElse() override {
        return it_ != jobs_.end();
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

    bool isCurrentJobModified_{false};
    unsigned currentJobIter_{0};
    typename std::list<Job*>::iterator it_;
};

} // namespace job_manager::detail

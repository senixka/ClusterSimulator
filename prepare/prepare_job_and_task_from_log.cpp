#include "../LogReader.h"
#include "../simulator/Defines.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

// Convert origin_time (which is in microseconds) to new_time = origin_time / TIME_SCALE
static const uint64_t TIME_SCALE{1'000'000};


struct LightTask {
    uint64_t time{0};
    uint64_t estimate{0}; // Microseconds
    unsigned cpuRequest{0};
    unsigned memoryRequest{0};
    unsigned eventType;

    LightTask(const LightTask&) = default;
    LightTask& operator=(const LightTask&) = default;

    LightTask(LightTask&&) = delete;
    LightTask& operator=(LightTask&&) = delete;

    explicit LightTask(const TaskEvent& taskEvent)
        : time(taskEvent.time),
          estimate{0},
          cpuRequest(static_cast<unsigned>(taskEvent.cpuRequest.value_or(0) * MACHINE_MAX_POSSIBLE_CPU +
                                           static_cast<long double>(0.5) / MACHINE_MAX_POSSIBLE_CPU)),
          memoryRequest(static_cast<unsigned>(taskEvent.memoryRequest.value_or(0) * MACHINE_MAX_POSSIBLE_MEMORY +
                                              static_cast<long double>(0.5) / MACHINE_MAX_POSSIBLE_MEMORY)),
          eventType(taskEvent.eventType) {
        cpuRequest = std::max(cpuRequest, 1u);
        memoryRequest = std::max(memoryRequest, 1u);
    }
};


using JobID_t = std::pair<uint64_t, std::string>;
//                          ^-- jobID      ^-- user
struct JobID_t_hash {
    std::size_t operator() (const JobID_t & p) const {
        return p.first;
    }
};


using TaskSpec_t = std::tuple<uint64_t, unsigned, unsigned>;
//                             ^- estimate  ^- cpu   ^-memory

struct OutJob {
    JobID_t jobID;
    std::map<TaskSpec_t, size_t> tasks;

    OutJob() = default;

    OutJob(const OutJob&) = default;
    OutJob& operator=(const OutJob&) = default;

    OutJob(OutJob&&) = delete;
    OutJob& operator=(OutJob&&) = delete;

    size_t TaskCount() const {
        size_t taskCount{0};
        for (const auto& [spec, nTask] : tasks) {
            taskCount += nTask;
        }

        return taskCount;
    }
};


void JobPrepare(OutJob& job, std::map<unsigned, std::list<LightTask>>& value, size_t& veryFastTaskCounter) {
    for (auto& [taskIndex, mutation] : value) {
        if (mutation.back().eventType <= TaskAndJobEventType::SCHEDULE) {
            mutation.back().estimate = UINT64_MAX;
            mutation.erase(mutation.cbegin(), std::prev(mutation.cend()));
        } else {
            uint64_t endTime = mutation.back().time;
            while (mutation.back().eventType != TaskAndJobEventType::SUBMIT) mutation.pop_back();
            uint64_t startTime = mutation.back().time;
            mutation.back().estimate = endTime - startTime;
        }

        assert(mutation.size() == 1);
        assert(mutation.back().cpuRequest > 0);
        assert(mutation.back().cpuRequest <= MACHINE_MAX_POSSIBLE_CPU);

        assert(mutation.back().memoryRequest > 0);
        assert(mutation.back().memoryRequest <= MACHINE_MAX_POSSIBLE_MEMORY);
    }
    assert(!value.empty());

    for (auto& [taskIndex, mutation] : value) {
        LightTask& task = mutation.front();
        TaskSpec_t key;

        if (task.estimate != UINT64_MAX) {
            key = {task.estimate / TIME_SCALE, task.cpuRequest, task.memoryRequest};
            assert(std::get<0>(key) < UINT32_MAX);
        } else {
            key = {UINT32_MAX, task.cpuRequest, task.memoryRequest};
        }

        if (0 < std::get<0>(key)) {
            ++job.tasks[key];
        } else {
            ++veryFastTaskCounter;
        }
    }
}

void JobDump(std::ofstream& out, const OutJob& job, uint64_t jobTime) {
    assert(jobTime < UINT32_MAX);
    out << jobTime << ' ' << job.tasks.size() << '\n';

    for (const auto& [spec, count] : job.tasks) {
        assert(std::get<0>(spec) <= UINT32_MAX);
        out << count << ' ' << std::get<0>(spec) << ' ' << std::get<1>(spec) << ' ' << std::get<2>(spec) << '\n';
    }
    out << '\n';
}


int main() {
    const std::string outputFilePath = "../../simulator/input/job_and_task.txt";
    const std::string inputLogDir = "../../google_cluster_trace_log/";

    const std::string sliceOutputFilePrefix = "../../simulator/input/job_and_task_slice_";
    const unsigned kSplits = 10;
    const unsigned sliceDuration = 7 * 24 * 60 * 60; // As output time_scale (now it is seconds)

    LogReader log(inputLogDir);

    std::unordered_map<JobID_t, uint64_t, JobID_t_hash> jobSubmitTime;
    //                             ^-- Microseconds
    {
        const JobID_t badJobKey{6253771429, "01B/AOjE02qBWt8YtZ1JWy7IG5LIf1FU6YSPUvJmcgA="}; // Reuse of JobID
        std::unordered_set<JobID_t, JobID_t_hash> jobWithMissingInfo;
        JobEvent jobEvent;

        while (log.NextJobEvent(jobEvent)) {
            if (jobEvent.eventType >= TaskAndJobEventType::UPDATE_PENDING) {
                continue;
            }

            assert(jobEvent.user.has_value());
            const JobID_t key{jobEvent.jobID, jobEvent.user.value()};

            if (key == badJobKey) [[unlikely]] {
                continue;
            }
            if (jobEvent.missingInfo.has_value()) [[unlikely]] {
                jobWithMissingInfo.insert(key);
            }

            if (jobSubmitTime.find(key) == jobSubmitTime.end()) {
                assert(jobEvent.eventType == TaskAndJobEventType::SUBMIT);
                jobSubmitTime[key] = jobEvent.time;
            } else {
                assert(jobEvent.eventType != TaskAndJobEventType::SUBMIT);
            }
        }

        std::cout << "JOB WITH MISSING INFO: " << jobWithMissingInfo.size() << std::endl;

        for (const auto &key: jobWithMissingInfo) {
            jobSubmitTime.erase(key);
        }

        std::cout << "JOB COUNT: " << jobSubmitTime.size() << std::endl;
    }


    auto cmpJobByTime = [&jobSubmitTime](const JobID_t &lhs, const JobID_t &rhs) {
        return std::tie(jobSubmitTime[lhs], lhs) < std::tie(jobSubmitTime[rhs], rhs);
    };
    std::map<JobID_t, std::map<unsigned, std::list<LightTask>>, decltype(cmpJobByTime)> taskEvents(cmpJobByTime);
    {
        log.VerboseFileProgress(true);

        TaskEvent taskEvent;
        std::set<std::pair<JobID_t, unsigned>> badTask;

        while (log.NextTaskEvent(taskEvent)) {
            if (taskEvent.eventType >= TaskAndJobEventType::UPDATE_PENDING) {
                continue;
            }

            assert(taskEvent.user.has_value());
            const JobID_t key{taskEvent.jobID, taskEvent.user.value()};

            if (taskEvent.missingInfo.has_value()) {
                badTask.insert({key, taskEvent.taskIndex});
            }

            assert(taskEvent.taskIndex < UINT32_MAX);
            if (taskEvent.eventType == TaskAndJobEventType::SUBMIT) {
                taskEvents[key][static_cast<unsigned>(taskEvent.taskIndex)].clear();
            }
            taskEvents[key][static_cast<unsigned>(taskEvent.taskIndex)].emplace_back(taskEvent);
        }

        std::cout << "TASK WITH MISSING INFO: " << badTask.size() << std::endl;

        for (const auto &[key, taskIndex]: badTask) {
            taskEvents[key].erase(taskIndex);
            if (taskEvents[key].empty()) {
                taskEvents.erase(key);
            }
        }

        for (auto it = taskEvents.begin(); it != taskEvents.end();) {
            const auto &[key, value] = *it;

            if (jobSubmitTime.find(key) == jobSubmitTime.end()) {
                it = taskEvents.erase(it);
            } else {
                ++it;
            }
        }

        std::set<uint64_t> jobID;
        for (const auto& [key, value] : taskEvents) {
            assert(jobID.find(key.first) == jobID.end());
            jobID.insert(key.first);
        }

        std::cout << "JOBS IN TASK_EVENTS: " << taskEvents.size() << std::endl;
    }


    std::list<OutJob> jobs;
    {
        // Prepare tasks in Job
        size_t veryFastTaskCounter{0};

        for (auto& [key, value]: taskEvents) {
            OutJob job;
            job.jobID = key;
            JobPrepare(job, value, veryFastTaskCounter);

            if (job.tasks.size() > 0) {
                jobs.push_back(job);
            }
        }

        std::cout << "VERY FAST TASK COUNTER: " << veryFastTaskCounter << std::endl;

        // Count tasks and scale Job time
        uint64_t lastJobTime{0}, taskCounter{0};
        for (const auto& outJob : jobs) {
            assert(jobSubmitTime[outJob.jobID] != UINT64_MAX);
            assert(jobSubmitTime[outJob.jobID] / TIME_SCALE < UINT32_MAX);

            jobSubmitTime[outJob.jobID] /= TIME_SCALE;

            assert(lastJobTime <= jobSubmitTime[outJob.jobID]);
            lastJobTime = jobSubmitTime[outJob.jobID];

            taskCounter += outJob.TaskCount();
        }
        assert(taskCounter < UINT32_MAX);


        // Write data to job_and_task file
        std::ofstream fout;
        fout.open(outputFilePath);

        fout << jobs.size() << ' ' << taskCounter << "\n\n";

        for (auto& outJob : jobs) {
            JobDump(fout, outJob, jobSubmitTime[outJob.jobID]);
        }

        fout.close();
    }

    {
        std::list<OutJob>::iterator left{jobs.begin()}, right;
        size_t nJob{0}, nTask{0};
        size_t zeroJob, zeroTask;
        {
            for (; left != jobs.end() && jobSubmitTime[left->jobID] == 0; ++left) {
                ++nJob;
                nTask += left->TaskCount();
            }

            assert(left != jobs.end());
            right = left;
            zeroJob = nJob;
            zeroTask = nTask;

            uint64_t rightTime = jobSubmitTime[left->jobID] + sliceDuration;
            for (; right != jobs.end() && jobSubmitTime[right->jobID] < rightTime; ++right) {
                ++nJob;
                nTask += right->TaskCount();
            }
        }

        const unsigned shift = static_cast<unsigned>(jobSubmitTime[jobs.back().jobID] -
                                                    (jobSubmitTime[left->jobID] + sliceDuration)) / kSplits;
        std::cout << "SHIFT IS: " << shift << std::endl;


        std::ofstream fout;
        for (unsigned i = 0; i < kSplits; ++i) {
            fout.open(sliceOutputFilePrefix + std::to_string(i) + ".txt");
            fout << nJob << ' ' << nTask << "\n\n";

            for (auto it = jobs.begin(); jobSubmitTime[it->jobID] == 0; ++it) {
                JobDump(fout, *it, 0);
            }
            for (auto it = left; it != right; ++it) {
                JobDump(fout, *it, jobSubmitTime[it->jobID] - jobSubmitTime[left->jobID]);
            }

            fout.close();

            uint64_t nextLeftTime = jobSubmitTime[left->jobID] + shift;
            for (; left != right && jobSubmitTime[left->jobID] < nextLeftTime; ++left) {
                --nJob;
                nTask -= left->TaskCount();
            }

            if (left == right) {
                assert(nTask == zeroTask);
                assert(nJob == zeroJob);
            }

            if (left == jobs.end()) {
                break;
            }

            uint64_t rightTime = jobSubmitTime[left->jobID] + sliceDuration;
            for (; right != jobs.end() && jobSubmitTime[right->jobID] < rightTime; ++right) {
                ++nJob;
                nTask += right->TaskCount();
            }
        }
    }

    return 0;
}

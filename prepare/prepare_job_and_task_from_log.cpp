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


using JobID_t = std::pair<uint64_t, std::string>;
struct JobID_t_hash {
    std::size_t operator() (const JobID_t & p) const {
        return p.first;
    }
};


class LightTask {
public:
    uint64_t time;
    uint64_t estimate;
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
          cpuRequest(taskEvent.cpuRequest.value_or(0) * MACHINE_MAX_POSSIBLE_CPU + static_cast<long double>(0.5) / MACHINE_MAX_POSSIBLE_CPU),
          memoryRequest(taskEvent.memoryRequest.value_or(0) * MACHINE_MAX_POSSIBLE_MEMORY + static_cast<long double>(0.5) / MACHINE_MAX_POSSIBLE_MEMORY),
          eventType(taskEvent.eventType) {
        cpuRequest = std::max(cpuRequest, 1u);
        memoryRequest = std::max(memoryRequest, 1u);
    }
};


void JobPrepareAndDump(std::ostream& out, uint64_t outJobID, uint64_t jobTime, std::map<unsigned, std::list<LightTask>>& value) {
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

    out << outJobID << ' ' << jobTime << '\n';
    out << value.size() << '\n';

    uint64_t outTaskIndex{0};
    for (const auto& [taskIndex, events] : value) {
        const auto& task = events.front();
        out << ++outTaskIndex << ' ' << task.estimate << ' ' << task.cpuRequest << ' ' << task.memoryRequest << '\n';
    }
    out << '\n';
}

int main() {
    const std::string outputFilePath = "../../simulator/input/job_and_task.txt";
    const std::string inputLogDir = "../../google_cluster_trace_log/";

    LogReader log(inputLogDir);

    std::unordered_map<JobID_t, uint64_t, JobID_t_hash> jobSubmitTime;
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
        std::string user;

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

            if (taskEvent.eventType == TaskAndJobEventType::SUBMIT) {
                taskEvents[key][taskEvent.taskIndex].clear();
            }
            taskEvents[key][taskEvent.taskIndex].emplace_back(taskEvent);
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


    {
        std::ofstream fout;
        fout.open(outputFilePath);

        fout << taskEvents.size() << "\n\n";

        uint64_t outJobID{0};
        for (auto &[key, value] : taskEvents) {
            JobPrepareAndDump(fout, ++outJobID, jobSubmitTime[key], value);
        }

        fout.close();
    }

    return 0;
}

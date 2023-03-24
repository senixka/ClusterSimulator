#include "../LogReader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <set>
#include <unordered_map>
#include <unordered_set>

class LightTask {
public:
    uint64_t time;
    uint64_t estimate = 0;
    double cpuRequest;
    double memoryRequest;
    double diskSpaceRequest;
    unsigned eventType;

    LightTask(const TaskEvent& taskEvent) :
        time(taskEvent.time),
        eventType(taskEvent.eventType) {
        cpuRequest = taskEvent.cpuRequest.has_value() ? taskEvent.cpuRequest.value() : 0;
        memoryRequest = taskEvent.memoryRequest.has_value() ? taskEvent.memoryRequest.value() : 0;
        diskSpaceRequest = taskEvent.diskSpaceRequest.has_value() ? taskEvent.diskSpaceRequest.value() : 0;
    }
};

class LightJob {
public:
    uint64_t time;
    unsigned char eventType;

    LightJob() = default;

    LightJob(const JobEvent& jobEvent)
        : time(jobEvent.time),
          eventType(jobEvent.eventType) {
        }
};


//void JobDump(ostream& out, string& user, uint64_t jobID, uint64_t jobTime, map<unsigned, list<Task>>& taskEvents) {
//    for (auto& it : taskEvents) {
//        list<Task>& mutation = it.second;
//
//        if (mutation.back().eventType <= TaskAndJobEventType::SCHEDULE) {
//            mutation.back().estimate = UINT64_MAX;
//        } else {
//            uint64_t endTime = mutation.back().time;
//            while (mutation.back().eventType != TaskAndJobEventType::SUBMIT) mutation.pop_back();
//            uint64_t startTime = mutation.back().time;
//            mutation.back().estimate = endTime - startTime;
//        }
//        mutation.erase(mutation.cbegin(), std::prev(mutation.cend()));
//    }
//
//    out << jobTime << ' ' << jobID << ' ' << user << '\n';
//    out << taskEvents.size() << '\n';
//
//    for (const auto& it : taskEvents) {
//        if (it.second.size() != 1) {
//            cout << "BAD TASK SIZE" << endl;
//        }
//
//        const Task& task = it.second.front();
//        out << task.estimate << ' ' << task.cpuRequest << ' ' << task.memoryRequest << ' ' << task.diskSpaceRequest << '\n';
//    }
//    out << '\n';
//}
//
//template <class T>
//inline void hash_combine(std::size_t& seed, const T& value) {
//    seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed<<6) + (seed>>2);
//}
//
//struct pair_hash {
//    template <class First, class Second>
//    std::size_t operator() (const std::pair<First, Second>& p) const {
//        std::size_t hash = 0;
//        hash_combine(hash, p.first);
//        hash_combine(hash, p.second);
//        return hash;
//    }
//};

int main() {
    const std::string outputFilePath = "../../simulator/input/machine.txt";
    const std::string inputLogDir = "../../google_cluster_trace_log/";

    using JobID_t = std::pair<uint64_t, std::string>;
    struct JobID_t_hash {
        std::size_t operator() (const JobID_t & p) const {
            return p.first;
        }
    };

    LogReader log(inputLogDir);
    log.VerboseFileProgress(true);


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
            if (jobEvent.missingInfo.has_value()) {
                jobWithMissingInfo.insert(key);
            }

            auto it = jobSubmitTime.find(key);
            if (it == jobSubmitTime.end()) {
                assert(jobEvent.eventType == TaskAndJobEventType::SUBMIT);
                jobSubmitTime[key] = jobEvent.time;
            } else {
                assert(jobEvent.eventType != TaskAndJobEventType::SUBMIT);
            }
        }

        std::cout << "JOBS IN JOB_EVENT: " << jobSubmitTime.size() << std::endl;
    }


//    std::unordered_map<JobID_t, std::map<unsigned, std::list<LightTask>>, JobID_t_hash> taskEvents;
//    {
//        TaskEvent taskEvent;
//        std::string user;
//
//        std::set<uint64_t> badTask;
//
//        while (log.NextTaskEvent(taskEvent)) {
//            if (taskEvent.eventType >= TaskAndJobEventType::UPDATE_PENDING) {
//                continue;
//            }
//
//            //user = taskEvent.user.has_value() ? taskEvent.user.value() : "NONAME";
//            //const JobID_t key{taskEvent.jobID, user};
//
//            if (!taskEvent.user.has_value()) {
//                badTask.insert(taskEvent.jobID);
//            }
//
//            //taskEvents[{taskEvent.jobID, user}][taskEvent.taskIndex].emplace_back(taskEvent);
//        }
//
//        std::cout << "JOBS IN TASK_EVENTS: " << taskEvents.size() << std::endl;
//
//        for (const auto& value : badTask) {
//            std::cout << value << " ";
//        }
//        std::cout << std::endl;
//        std::cout << "BAD: " << badTask.size() << std::endl;
//    }
//
//    // Different checks
//    {
//        for (const auto& it : task_events) {
//            pair<uint64_t, string> key = it.first;
//            uint64_t jobID = key.first;
//            string user = key.second;
//
//            if (it.second.empty()) {
//                cout << "EMPTY JOB: " << jobID << " " << user << endl;
//                continue;
//            }
//
//            for (const auto& inner : it.second) {
//                unsigned taskIndex = inner.first;
//
//                if (inner.second.empty()) {
//                    cout << "EMPTY TASK: " << jobID << " " << user << " " << taskIndex << endl;
//                    continue;
//                }
//
//                if (inner.second.front().eventType != TaskAndJobEventType::SUBMIT) {
//                    cout << "NO SUBMIT: " << jobID << " " << user << " " << taskIndex << endl;
//                }
//
//                for (auto elem = inner.second.begin(); elem != inner.second.end(); ++elem) {
//                    if (elem->eventType == TaskAndJobEventType::SCHEDULE) {
//                        --elem;
//                        if (elem->eventType != TaskAndJobEventType::SUBMIT) {
//                            cout << "NO SUBMIT BEFORE SCHEDULE (" << elem->eventType << ") " << jobID << " " << user << " " << taskIndex << endl;
//                        }
//                        ++elem;
//                    }
//
//                    if (elem->eventType >= 3) { // REAL CASE: 0 2 0 1 4
//                        --elem;
//                        if (elem->eventType > TaskAndJobEventType::SCHEDULE) {
//                            cout << "NO SCHEDULE OR SUBMIT BEFORE DEAD (" << elem->eventType << ") " << jobID << " " << user << " " << taskIndex << endl;
//                        }
//                        ++elem;
//                    }
//                }
//            }
//        }
//
//        /*
//            - No empty jobs.
//            - No empty tasks.
//            - There is an immediate SUBMIT before any SCHEDULE.
//            - There is always a SUBMIT or SCHEDULE before the dead event with code >=3.
//        */
//    }
//
//    ofstream fout;
//    fout.open("prepared.txt");
//
//    fout << task_events.size() << "\n\n";
//
//    for (auto& it : task_events) {
//        pair<uint64_t, string> key = it.first;
//        uint64_t jobID = key.first;
//        string user = key.second;
//
//        JobDump(fout, user, jobID, jobToTime[{jobID, user}], it.second);
//    }
//    fout.close();

    return 0;
}

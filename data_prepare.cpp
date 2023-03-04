#include "LogReader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;


class Task {
public:
    uint64_t time;
    uint64_t estimate = 0;
    double cpuRequest;
    double memoryRequest;
    double diskSpaceRequest;
    unsigned eventType;

    Task(const TaskEvent& taskEvent) :
        time(taskEvent.time),
        eventType(taskEvent.eventType) {
        cpuRequest = taskEvent.cpuRequest.has_value() ? taskEvent.cpuRequest.value() : 0;
        memoryRequest = taskEvent.memoryRequest.has_value() ? taskEvent.memoryRequest.value() : 0;
        diskSpaceRequest = taskEvent.diskSpaceRequest.has_value() ? taskEvent.diskSpaceRequest.value() : 0;
    }
};

class Job {
public:
    uint64_t time;
    unsigned char eventType;

    Job(const JobEvent& jobEvent) :
        time(jobEvent.time),
        eventType(jobEvent.eventType) {
        }
};


void JobDump(ostream& out, string& user, uint64_t jobID, uint64_t jobTime, map<unsigned, list<Task>>& taskEvents) {
    for (auto& it : taskEvents) {
        list<Task>& mutation = it.second;

        if (mutation.back().eventType <= EventType::SCHEDULE) {
            mutation.back().estimate = UINT64_MAX;
        } else {
            uint64_t endTime = mutation.back().time;
            while (mutation.back().eventType != EventType::SUBMIT) mutation.pop_back();
            uint64_t startTime = mutation.back().time;
            mutation.back().estimate = endTime - startTime;
        }
        mutation.erase(mutation.cbegin(), std::prev(mutation.cend()));
    }

    out << jobTime << ' ' << jobID << ' ' << user << '\n';
    out << taskEvents.size() << '\n';

    for (const auto& it : taskEvents) {
        if (it.second.size() != 1) {
            cout << "BAD TASK SIZE" << endl;
        }

        const Task& task = it.second.front();
        out << task.estimate << ' ' << task.cpuRequest << ' ' << task.memoryRequest << ' ' << task.diskSpaceRequest << '\n';
    }
    out << '\n';
}

int main() {
    LogReader log;
    
    map<pair<uint64_t, string>, list<Job>> job_events;
    {
        string user;
        JobEvent jobEvent;
        while (log.NextJobEvent(jobEvent)) {
            if (jobEvent.eventType >= EventType::UPDATE_PENDING) continue;

            user = jobEvent.user.has_value() ? jobEvent.user.value() : "NONAME";
            job_events[{jobEvent.jobID, user}].emplace_back(jobEvent);
        }

        cout << "JOBS IN JOB_EVENT: " << job_events.size() << endl;
    }

    map<pair<uint64_t, string>, uint64_t> jobToTime;
    {
        for (const auto& it : job_events) {
            jobToTime[it.first] = it.second.front().time;
        }

        // First event in job must be SUBMIT
        for (const auto& it : job_events) {
            if (it.second.front().eventType != EventType::SUBMIT) {
                cout << "NO SUBMIT " << it.first.first << " " << it.first.second << endl;
            }
        }
    }

    auto cmp = [&jobToTime](const pair<uint64_t, string>& lhs, const pair<uint64_t, string>& rhs) -> int {
        return std::tie(jobToTime[lhs], lhs) < std::tie(jobToTime[rhs], rhs);
    };
    map<pair<uint64_t, string>, map<unsigned, list<Task>>, decltype(cmp)> task_events(cmp);
    {
        string user;
        TaskEvent taskEvent;
        while (log.NextTaskEvent(taskEvent)) {
            if (taskEvent.eventType >= 7) continue;

            user = taskEvent.user.has_value() ? taskEvent.user.value() : "NONAME";
            task_events[{taskEvent.jobID, user}][taskEvent.taskIndex].emplace_back(taskEvent);
        }

        cout << "JOBS IN TASK_EVENTS: " << task_events.size() << endl;
    }

    // Different checks
    {
        for (const auto& it : task_events) {
            pair<uint64_t, string> key = it.first;
            uint64_t jobID = key.first;
            string user = key.second;

            if (it.second.empty()) {
                cout << "EMPTY JOB: " << jobID << " " << user << endl;
                continue;
            }

            for (const auto& inner : it.second) {
                unsigned taskIndex = inner.first;

                if (inner.second.empty()) {
                    cout << "EMPTY TASK: " << jobID << " " << user << " " << taskIndex << endl;
                    continue;
                }

                if (inner.second.front().eventType != EventType::SUBMIT) {
                    cout << "NO SUBMIT: " << jobID << " " << user << " " << taskIndex << endl;
                }

                for (auto elem = inner.second.begin(); elem != inner.second.end(); ++elem) {
                    if (elem->eventType == EventType::SCHEDULE) {
                        --elem;
                        if (elem->eventType != EventType::SUBMIT) {
                            cout << "NO SUBMIT BEFORE SCHEDULE (" << elem->eventType << ") " << jobID << " " << user << " " << taskIndex << endl;
                        }
                        ++elem;
                    }

                    if (elem->eventType >= 3) { // REAL CASE: 0 2 0 1 4
                        --elem;
                        if (elem->eventType > EventType::SCHEDULE) {
                            cout << "NO SCHEDULE OR SUBMIT BEFORE DEAD (" << elem->eventType << ") " << jobID << " " << user << " " << taskIndex << endl;
                        }
                        ++elem;
                    }
                }
            }
        }

        /*
            - No empty jobs.
            - No empty tasks.
            - There is an immediate SUBMIT before any SCHEDULE.
            - There is always a SUBMIT or SCHEDULE before the dead event with code >=3.
        */
    }

    ofstream fout;
    fout.open("prepared.txt");

    fout << task_events.size() << "\n\n";

    for (auto& it : task_events) {
        pair<uint64_t, string> key = it.first;
        uint64_t jobID = key.first;
        string user = key.second;

        JobDump(fout, user, jobID, jobToTime[{jobID, user}], it.second);
    }
    fout.close();

    return 0;
}

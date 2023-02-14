#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <optional>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <list>

using namespace std;

enum EventType {
    /*
    ● SUBMIT(0): A task or job became eligible for scheduling.
    ● SCHEDULE(1): A job or task was scheduled on a machine. (It may not start running
      immediately due to code-shipping time, etc.) For jobs, this occurs the first time any
      task of the job is scheduled on a machine.
    ● EVICT(2): A task or job was descheduled because of a higher priority task or job,
      because the scheduler overcommitted and the actual demand exceeded the machine
      capacity, because the machine on which it was running became unusable (e.g. taken
      offline for repairs), or because a disk holding the task’s data was lost.
    ● FAIL(3): A task or job was descheduled (or, in rare cases, ceased to be eligible for
      scheduling while it was pending) due to a task failure.
    ● FINISH(4): A task or job completed normally.
    ● KILL(5): A task or job was cancelled by the user or a driver program or because
      another job or task on which this job was dependent died.
    ● LOST(6): A task or job was presumably terminated, but a record indicating its
      termination was missing from our source data.
    ● UPDATE_PENDING(7): A task or job’s scheduling class, resource requirements, or
      constraints were updated while it was waiting to be scheduled.
    ● UPDATE_RUNNING(8): A task or job’s scheduling class, resource requirements, or
      constraints were updated while it was scheduled.
    */

    SUBMIT = 0,
    SCHEDULE = 1,
    EVICT = 2,
    FAIL = 3,
    FINISH = 4,
    KILL = 5,
    LOST = 6,
    UPDATE_PENDING = 7,
    UPDATE_RUNNING = 8,
};

struct JobEvent {
/*
    file pattern	                        field number	content	            format	    mandatory
    job_events/part-?????-of-?????.csv.gz	1	            time	            INTEGER	    YES
    job_events/part-?????-of-?????.csv.gz	2	            missing info	    INTEGER	    NO
    job_events/part-?????-of-?????.csv.gz	3	            job ID	            INTEGER	    YES
    job_events/part-?????-of-?????.csv.gz	4	            event type	        INTEGER	    YES
    job_events/part-?????-of-?????.csv.gz	5	            user	            STRING_HASH	NO
    job_events/part-?????-of-?????.csv.gz	6	            scheduling class	INTEGER	    NO
    job_events/part-?????-of-?????.csv.gz	7	            job name	        STRING_HASH	NO
    job_events/part-?????-of-?????.csv.gz	8	            logical job name	STRING_HASH	NO
*/

    uint64_t time;
    optional<uint64_t> missingInfo;
    uint64_t jobID;
    EventType eventType;
    optional<string> user;
    optional<uint64_t> schedulingClass;
    optional<string> jobName;
    optional<string> logicalJobName;

    friend ostream& operator<<(ostream& out, const JobEvent& job) {
        out << "JOB_EVENT\tTime: " << job.time << "\tJobID: " << job.jobID << "\tEventType: " << job.eventType;
        return out;
    }
};

struct TaskEvent {
/*
    file pattern	                        field number	content	            format	    mandatory
    task_events/part-?????-of-?????.csv.gz	1	            time	            INTEGER	    YES
    task_events/part-?????-of-?????.csv.gz	2	            missing info	    INTEGER	    NO
    task_events/part-?????-of-?????.csv.gz	3	            job ID	            INTEGER	    YES
    task_events/part-?????-of-?????.csv.gz	4	            task index	        INTEGER	    YES
    task_events/part-?????-of-?????.csv.gz	5	            machine ID	        INTEGER	    NO
    task_events/part-?????-of-?????.csv.gz	6	            event type	        INTEGER	    YES
    task_events/part-?????-of-?????.csv.gz	7	            user	            STRING_HASH	NO
    task_events/part-?????-of-?????.csv.gz	8	            scheduling class	INTEGER	    NO
    task_events/part-?????-of-?????.csv.gz	9	            priority	        INTEGER	    YES
    task_events/part-?????-of-?????.csv.gz	10	            CPU request	        FLOAT	    NO
    task_events/part-?????-of-?????.csv.gz	11	            memory request	    FLOAT	    NO
    task_events/part-?????-of-?????.csv.gz	12	            disk space request	FLOAT	    NO
    task_events/part-?????-of-?????.csv.gz	13	            different machines restriction	BOOLEAN	NO
 */

    uint64_t time; // max = 18446744073709551615
    optional<uint64_t> missingInfo;
    uint64_t jobID;
    uint64_t taskIndex; // max = 90049
    optional<uint64_t> machineID;
    EventType eventType;
    optional<string> user;
    optional<uint64_t> schedulingClass;
    uint64_t priority; // max = 11
    optional<long double> cpuRequest;
    optional<long double> memoryRequest;
    optional<long double> diskSpaceRequest;
    optional<bool> differentMachinesRestriction;

    friend ostream& operator<<(ostream& out, const TaskEvent& task) {
        out << "TASK_EVENT\tTime: " << task.time << "\tJobID: " << task.jobID << "\tTaskIndex: " << task.taskIndex << "\tEventType: " << task.eventType;
        return out;
    }
};

class LogReader {
public:
    const filesystem::path pathJobEvents = R"(./data/job_events)";
    const filesystem::path pathTaskEvents = R"(./data/task_events)";
    vector<filesystem::path> jobEventFiles;
    vector<filesystem::path> taskEventFiles;
    ifstream jobFin;
    ifstream taskFin;

    LogReader() {
        for (const auto& entry : filesystem::directory_iterator(pathJobEvents)) {
            jobEventFiles.push_back(entry.path());
        }
        sort(jobEventFiles.begin(), jobEventFiles.end());

        for (const auto& entry : filesystem::directory_iterator(pathTaskEvents)) {
            taskEventFiles.push_back(entry.path());
        }
        sort(taskEventFiles.begin(), taskEventFiles.end());
    }

    bool NextJobEvent(JobEvent& jobEvent) {
        if (jobEventFiles.empty()) return false;
        if (!jobFin.is_open()) jobFin.open(jobEventFiles.front());

        string line;
        getline(jobFin, line);
        if (line.empty()) {
            jobFin.close();
            jobEventFiles.erase(jobEventFiles.cbegin());
            return NextJobEvent(jobEvent);
        }

        stringstream ss(line);
        vector<string> params(8);

        string token;
        for (size_t i = 0; i < 8; ++i) {
            getline(ss, token, ',');
            params[i] = token;
        }

        jobEvent.missingInfo.reset();
        jobEvent.user.reset();
        jobEvent.schedulingClass.reset();
        jobEvent.jobName.reset();
        jobEvent.logicalJobName.reset();

        jobEvent.time = stoull(params[0]);
        if (!params[1].empty()) jobEvent.missingInfo = stoull(params[1]);
        jobEvent.jobID = stoull(params[2]);
        jobEvent.eventType = static_cast<EventType>(stoi(params[3]));
        if (!params[4].empty()) jobEvent.user = params[4];
        if (!params[5].empty()) jobEvent.schedulingClass = stoull(params[5]);
        if (!params[6].empty()) jobEvent.jobName = params[6];
        if (!params[7].empty()) jobEvent.logicalJobName = params[7];

        return true;
    }

    bool NextTaskEvent(TaskEvent& taskEvent) {
        if (taskEventFiles.empty()) return false;
        if (!taskFin.is_open()) taskFin.open(taskEventFiles.front()), cout << taskEventFiles.front() << endl;

        string line;
        getline(taskFin, line);
        if (line.empty()) {
            taskFin.close();
            taskEventFiles.erase(taskEventFiles.cbegin());
            return NextTaskEvent(taskEvent);
        }

        stringstream ss(line);
        vector<string> params(13);

        string token;
        for (size_t i = 0; i < 13; ++i) {
            getline(ss, token, ',');
            params[i] = token;
        }
        
        taskEvent.missingInfo.reset();
        taskEvent.machineID.reset();
        taskEvent.user.reset();
        taskEvent.schedulingClass.reset();
        taskEvent.cpuRequest.reset();
        taskEvent.memoryRequest.reset();
        taskEvent.diskSpaceRequest.reset();
        taskEvent.differentMachinesRestriction.reset();

        taskEvent.time = stoull(params[0]);
        if (!params[1].empty()) taskEvent.missingInfo = stoull(params[1]);
        taskEvent.jobID = stoull(params[2]);
        taskEvent.taskIndex = stoull(params[3]);
        if (!params[4].empty()) taskEvent.machineID = stoull(params[4]);
        taskEvent.eventType = static_cast<EventType>(stoi(params[5]));
        if (!params[6].empty()) taskEvent.user = params[6];
        if (!params[7].empty()) taskEvent.schedulingClass = stoull(params[7]);
        taskEvent.priority = stoull(params[8]);
        if (!params[9].empty()) taskEvent.cpuRequest = stold(params[9]);
        if (!params[10].empty()) taskEvent.memoryRequest = stold(params[10]);
        if (!params[11].empty()) taskEvent.diskSpaceRequest = stold(params[11]);
        if (!params[12].empty()) taskEvent.differentMachinesRestriction = stoi(params[12]);

        return true;
    }

    ~LogReader() {
        if (jobFin.is_open()) jobFin.close();
        if (taskFin.is_open()) taskFin.close();
    }
};

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

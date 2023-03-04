#include "LogReader.h"
#include "matplotlibcpp.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

namespace plt = matplotlibcpp;
using namespace std;

const uint64_t SPEEDUP_JOB_TIME = 1;
const uint64_t SPEEDUP_TASK_TIME = 1;

class Machine {
public:
    uint64_t time;
    uint64_t machineID;
    double cpu;
    double memory;
    unsigned eventType;

    Machine() = default;

    Machine(const MachineEvent& machineEvent) :
            time(machineEvent.time),
            machineID(machineEvent.machineID),
            eventType(machineEvent.eventType) {
        cpu = machineEvent.cpu.has_value() ? machineEvent.cpu.value() : 0;
        memory = machineEvent.memory.has_value() ? machineEvent.memory.value() : 0;
        if (cpu == 0 || memory == 0) {
            std::cout << "NO DATA CPU or MEM in MACHINE_EVENTS " << cpu << " " << memory << std::endl;
        }
    }
};

class Task {
public:
    uint64_t estimate = 0;
    uint64_t endTime = 0;
    double cpuRequest = 0;
    double memoryRequest = 0;
    double diskSpaceRequest = 0;
    unsigned machineIndex = UINT32_MAX;

    friend bool operator>(const Task& lhs, const Task& rhs) {
        return lhs.endTime > rhs.endTime;
    }
};

class JobMaster {
public:
    uint64_t jobTime = 0, jobID = 0;
    string user;
    list<Task> pendingTask;

    JobMaster() = default;

    JobMaster(istream& in) {
        in >> jobTime >> jobID >> user;
        jobTime /= SPEEDUP_JOB_TIME;

        size_t taskN;
        in >> taskN;

        Task task;
        for (size_t i = 0; i < taskN; ++i) {
            in >> task.estimate >> task.cpuRequest >> task.memoryRequest >> task.diskSpaceRequest;
            pendingTask.push_back(task);
        }
    }
};

void MachineEventStat() {
    LogReader log;

    vector<uint64_t> cnt_stat, timing;
    vector<double> cpu_stat, mem_stat;

    uint64_t temp_time = 0;
    const uint64_t each_time = 10'000'000;
    const int error_exit = 23;
    long double cpu = 0, mem = 0;

    MachineEvent machineEvent;
    unordered_map<uint64_t, Machine> machine_events;

    while (log.NextMachineEvent(machineEvent)) {
        Machine machine{machineEvent};
        auto it = machine_events.find(machine.machineID);

        if (machineEvent.eventType == MachineEventType::ADD) {
            if (it != machine_events.end()) {
                std::cout << "MACHINE ALREADY EXIST" << std::endl;
                exit(error_exit);
            }
            cpu += machine.cpu;
            mem += machine.memory;
            machine_events[machineEvent.machineID] = machine;
        } else if (machineEvent.eventType == MachineEventType::UPDATE) {
            cpu -= it->second.cpu;
            mem -= it->second.memory;
            it->second = machine;
            cpu += machine.cpu;
            mem += machine.memory;
        } else if (machineEvent.eventType == MachineEventType::REMOVE) {
            cpu -= it->second.cpu;
            mem -= it->second.memory;
            machine_events.erase(it);
        } else {
            std::cout << "UNEXPECTED MACHINE TYPE " << machineEvent.eventType << std::endl;
            exit(error_exit);
        }

        while (machineEvent.time != UINT64_MAX && machineEvent.time > temp_time) {
            cnt_stat.push_back(machine_events.size());
            cpu_stat.push_back(cpu);
            mem_stat.push_back(mem);

            timing.push_back(temp_time);
            temp_time += each_time;
        }
    }

    uint64_t cnt_avg = std::accumulate(cnt_stat.begin(), cnt_stat.end(), 0ull) / cnt_stat.size();
    double cpu_avg = std::accumulate(cpu_stat.begin(), cpu_stat.end(), static_cast<long double>(0)) / cpu_stat.size();
    double mem_avg = std::accumulate(mem_stat.begin(), mem_stat.end(), static_cast<long double>(0)) / mem_stat.size();

    plt::figure_size(1000, 400);
    plt::title("Number of Machines in Time (average at " + to_string(cnt_avg) + ")");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Number of machines");
    plt::plot(timing, cnt_stat, {{"label", "Number"}});
    plt::plot(timing, vector<uint64_t>(timing.size(), cnt_avg), {{"label", "NUmber AVG at " + to_string(cnt_avg)}});
    plt::legend();
    plt::savefig("./stat/machine_number.png");
    plt::show();

    plt::figure_size(1000, 400);
    plt::title("Machine resource Units in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Resource Units");
    plt::plot(timing, cpu_stat, {{"label", "CPU"}});
    plt::plot(timing, vector(timing.size(), cpu_avg), {{"label", "CPU AVG at " + to_string(cpu_avg)}});
    plt::plot(timing, mem_stat, {{"label", "Memory"}});
    plt::plot(timing, vector(timing.size(), mem_avg), {{"label", "Memory AVG at " + to_string(mem_avg)}});
    plt::legend();
    plt::savefig("./stat/machine_resources.png");
    plt::show();
}

void TaskEventStatInPreparedData() {
    LogReader log;

    vector<uint64_t> cnt_stat;
    vector<double> cpu_stat, mem_stat, disk_stat;

    vector<uint64_t> timing;
    uint64_t temp_time = 0;
    uint64_t each_time = 10'000'000;

    uint64_t cnt = 0;
    long double cpu = 0, mem = 0, disk = 0;

    auto UpdateStat = [&](uint64_t endTime) -> void {
        while (endTime != UINT64_MAX && endTime > temp_time) {
            cnt_stat.push_back(cnt);
            cpu_stat.push_back(cpu);
            mem_stat.push_back(mem);
            disk_stat.push_back(disk);

            timing.push_back(temp_time);
            temp_time += each_time;
        }
    };

    ifstream fin;
    fin.open("./prepared.txt");

    size_t jobN;
    fin >> jobN;

    priority_queue<Task, vector<Task>, greater<>> runningTask;

    for (size_t i = 0; i < jobN; ++i) {
        JobMaster jm(fin);

        if (i % 10'000 == 0) {
            cout << i << "/" << jobN << endl;
        }

        while (!runningTask.empty() && runningTask.top().endTime != UINT64_MAX && jm.jobTime > runningTask.top().endTime) {
            Task task{runningTask.top()};
            runningTask.pop();

            --cnt;
            cpu -= task.cpuRequest;
            mem -= task.memoryRequest;
            disk -= task.diskSpaceRequest;

            UpdateStat(task.endTime);
        }

        for (auto& task : jm.pendingTask) {
            ++cnt;
            cpu += task.cpuRequest;
            mem += task.memoryRequest;
            disk += task.diskSpaceRequest;

            task.endTime = task.estimate != UINT64_MAX ? jm.jobTime + task.estimate / SPEEDUP_TASK_TIME : UINT64_MAX;
            runningTask.push(task);
        }

        UpdateStat(jm.jobTime);
    }

    fin.close();

    uint64_t cnt_avg = std::accumulate(cnt_stat.begin(), cnt_stat.end(), 0ull) / cnt_stat.size();
    double cpu_avg = std::accumulate(cpu_stat.begin(), cpu_stat.end(), static_cast<long double>(0)) / cpu_stat.size();
    double mem_avg = std::accumulate(mem_stat.begin(), mem_stat.end(), static_cast<long double>(0)) / mem_stat.size();
    double disk_avg = std::accumulate(disk_stat.begin(), disk_stat.end(), static_cast<long double>(0)) / disk_stat.size();

    plt::figure_size(1000, 400);
    plt::title("Number of Tasks in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Task number");
    plt::plot(timing, cnt_stat, {{"label", "Number"}});
    plt::plot(timing, vector<uint64_t>(timing.size(), cnt_avg), {{"label", "Number AVG at " + to_string(cnt_avg)}});
    plt::legend();
    plt::savefig("./stat/task_number_speedup_job_task_" + to_string(SPEEDUP_JOB_TIME) + "_" + to_string(SPEEDUP_TASK_TIME) + ".png");
    plt::show();

    plt::figure_size(1200, 500);
    plt::title("Task resource Units demand in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Resource Units");
    plt::plot(timing, cpu_stat, {{"label", "CPU"}});
    plt::plot(timing, vector(timing.size(), cpu_avg), {{"label", "CPU AVG at " + to_string(cpu_avg)}});
    plt::plot(timing, mem_stat, {{"label", "Memory"}});
    plt::plot(timing, vector(timing.size(), mem_avg), {{"label", "Memory AVG at " + to_string(mem_avg)}});
    plt::plot(timing, disk_stat, {{"label", "Disk"}});
    plt::plot(timing, vector(timing.size(), disk_avg), {{"label", "Disk AVG at " + to_string(disk_avg)}});
    plt::legend();
    plt::savefig("./stat/task_resources_speedup_job_task_" + to_string(SPEEDUP_JOB_TIME) + "_" + to_string(SPEEDUP_TASK_TIME) + ".png");
    plt::show();
}

void TaskEventStatInLogData() {
    LogReader log;
    log.VerboseFileProgress(true);

    vector<uint64_t> cnt_stat;
    vector<double> cpu_stat, mem_stat, disk_stat;

    vector<uint64_t> timing;
    uint64_t temp_time = 0;
    uint64_t each_time = 500'000'000;

    uint64_t cnt = 0;
    long double cpu = 0, mem = 0, disk = 0;

    struct pair_hash {
        size_t operator () (const std::pair<uint64_t, unsigned>& p) const {
            const size_t h1 = std::hash<size_t>()(p.first);
            const size_t h2 = std::hash<size_t>()(p.second);
            return h1 ^ h2;
        }
    };

    TaskEvent taskEvent;
    unordered_map<pair<uint64_t, unsigned>, tuple<double, double, double>, pair_hash> task_to_resource;

    while (log.NextTaskEvent(taskEvent)) {
        TaskAndJobEventType eventType = taskEvent.eventType;
        auto key = make_pair(taskEvent.jobID, taskEvent.taskIndex);
        double cpuRequest = taskEvent.cpuRequest.has_value() ? taskEvent.cpuRequest.value() : 0;
        double memoryRequest = taskEvent.memoryRequest.has_value() ? taskEvent.memoryRequest.value() : 0;
        double diskSpaceRequest = taskEvent.diskSpaceRequest.has_value() ? taskEvent.diskSpaceRequest.value() : 0;

        if (eventType == TaskAndJobEventType::SUBMIT || eventType == TaskAndJobEventType::UPDATE_PENDING) {
            continue;
        } else if (eventType == TaskAndJobEventType::SCHEDULE) {
            ++cnt;
            cpu += cpuRequest;
            mem += memoryRequest;
            disk += diskSpaceRequest;

            task_to_resource[key] = {cpuRequest, memoryRequest, diskSpaceRequest};
        } else if (TaskAndJobEventType::SCHEDULE < eventType && eventType <= TaskAndJobEventType::LOST) {
            auto it = task_to_resource.find(key);

            if (it != task_to_resource.end()) {
                --cnt;
                cpu -= get<0>(it->second);
                mem -= get<1>(it->second);
                disk -= get<2>(it->second);

                task_to_resource.erase(it);
            }

        } else if (eventType == TaskAndJobEventType::UPDATE_RUNNING) {
            auto it = task_to_resource.find(key);
            if (it == task_to_resource.end()) {
                cout << "BAD it in UPDATE_RUNNING" << endl;
            }

            cpu -= get<0>(it->second);
            mem -= get<1>(it->second);
            disk -= get<2>(it->second);

            cpu += cpuRequest;
            mem += memoryRequest;
            disk += diskSpaceRequest;

            it->second = {cpuRequest, memoryRequest, diskSpaceRequest};
        } else {
            cout << "Unexpected task event type" << endl;
        }

        while (taskEvent.time != UINT64_MAX && taskEvent.time > temp_time) {
            cnt_stat.push_back(cnt);
            cpu_stat.push_back(cpu);
            mem_stat.push_back(mem);
            disk_stat.push_back(disk);

            timing.push_back(temp_time);
            temp_time += each_time;
        }
    }

    uint64_t cnt_avg = std::accumulate(cnt_stat.begin(), cnt_stat.end(), 0ull) / cnt_stat.size();
    double cpu_avg = std::accumulate(cpu_stat.begin(), cpu_stat.end(), static_cast<long double>(0)) / cpu_stat.size();
    double mem_avg = std::accumulate(mem_stat.begin(), mem_stat.end(), static_cast<long double>(0)) / mem_stat.size();
    double disk_avg = std::accumulate(disk_stat.begin(), disk_stat.end(), static_cast<long double>(0)) / disk_stat.size();

    plt::figure_size(1000, 400);
    plt::title("Number of Tasks in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Task number");
    plt::plot(timing, cnt_stat, {{"label", "Number"}});
    plt::plot(timing, vector<uint64_t>(timing.size(), cnt_avg), {{"label", "Number AVG at " + to_string(cnt_avg)}});
    plt::legend();
    plt::savefig("./stat/log_task_number.png");
    plt::show();

    plt::figure_size(1200, 500);
    plt::title("Task resource Units demand in Time");
    plt::xlabel("Time (in microseconds)");
    plt::ylabel("Resource Units");
    plt::plot(timing, cpu_stat, {{"label", "CPU"}});
    plt::plot(timing, vector(timing.size(), cpu_avg), {{"label", "CPU AVG at " + to_string(cpu_avg)}});
    plt::plot(timing, mem_stat, {{"label", "Memory"}});
    plt::plot(timing, vector(timing.size(), mem_avg), {{"label", "Memory AVG at " + to_string(mem_avg)}});
    plt::plot(timing, disk_stat, {{"label", "Disk"}});
    plt::plot(timing, vector(timing.size(), disk_avg), {{"label", "Disk AVG at " + to_string(disk_avg)}});
    plt::legend();
    plt::savefig("./stat/log_task_resources.png");
    plt::show();
}

int main() {
    // MachineEventStat();

    TaskEventStatInPreparedData();

    // TaskEventStatInLogData();

    return 0;
}

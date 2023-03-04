#include "matplotlibcpp.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace plt = matplotlibcpp;
using namespace std;

const uint64_t SPEEDUP_JOB_TIME = 1;
const uint64_t SPEEDUP_TASK_TIME = 1;

struct Task {
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

class Machine {
public:
    double cpuCapacity = 1;
    double memoryCapacity = 1;
    double diskSpaceCapacity = 1;

    bool IsPlaceable(const Task& task) const {
        return task.cpuRequest <= cpuCapacity
            && task.memoryRequest <= memoryCapacity
            && task.diskSpaceRequest <= diskSpaceCapacity;
    }

    void PlaceTask(const Task& task) {
        cpuCapacity -= task.cpuRequest;
        memoryCapacity -= task.memoryRequest;
        diskSpaceCapacity -= task.diskSpaceRequest;
    }

    void RemoveTask(const Task& task) {
        cpuCapacity += task.cpuRequest;
        memoryCapacity += task.memoryRequest;
        diskSpaceCapacity += task.diskSpaceRequest;
    }
};

class Cluster {
    uint64_t time = 0;
    uint64_t updateIteration = 0;
    const uint64_t scheduleEach = 1000;

    vector<Machine> machine;

    list<JobMaster> futureJobs;
    list<JobMaster> currentJobs;
    priority_queue<Task, vector<Task>, greater<>> runningTask;

    long double usedCPU = 0;
    long double usedMemory = 0;
    long double usedDisk = 0;

    long double totalCPU = 0;
    long double totalMemory = 0;
    long double totalDisk = 0;
    
    const uint64_t updateStatEach = 10'000'000;
    uint64_t updateStatTime = 0;
    
    vector<float> statCPU;
    vector<float> statMemory;
    vector<float> statDisk;
    vector<uint64_t> statTime;
    

public:
    Cluster(size_t machineN = 12500) {
        machine.resize(machineN);
        totalCPU = machine[0].cpuCapacity * machineN;
        totalMemory = machine[0].memoryCapacity * machineN;
        totalDisk = machine[0].diskSpaceCapacity * machineN;

        ifstream fin;
        fin.open("./prepared.txt");

        size_t jobN;
        fin >> jobN;

        for (size_t i = 0; i < jobN; ++i) {
            futureJobs.emplace_back(std::move(JobMaster(fin)));
        }

        fin.close();
    }

    void Run() {
        while (Update()) {
        }
        
        DumpStat();
    }

    bool Update() {
        ++updateIteration;

        if (currentJobs.empty() && futureJobs.empty() && runningTask.empty()) {
            return false;
        }

        uint64_t futureJobTime = futureJobs.empty() ? UINT64_MAX : futureJobs.front().jobTime;
        uint64_t futureTaskTime = runningTask.empty() ? UINT64_MAX : runningTask.top().endTime;

        if (futureJobTime < futureTaskTime || (futureJobTime == futureTaskTime && futureJobTime != UINT64_MAX)) {
            JobMaster& job = futureJobs.front();
            time = job.jobTime;
            currentJobs.push_front(job);
            futureJobs.pop_front();
        } else if (futureTaskTime < futureJobTime) {
            Task task = runningTask.top();
            runningTask.pop();
            time = task.endTime;
            RemoveTaskFromMachine(task);
        } else {
            return false;
        }

        if (updateIteration % scheduleEach == 1) {
            DeleteFinishedJobs();
            PrintClusterStat();
            Schedule();
        }
        
        while (time > updateStatTime) {
            statCPU.push_back(usedCPU * 100 / totalCPU);
            statMemory.push_back(usedMemory * 100 / totalMemory);
            statDisk.push_back(usedDisk * 100 / totalDisk);
            statTime.push_back(updateStatTime);
            
            updateStatTime += updateStatEach;
        }

        return true;
    }

    void Schedule() {
        for (auto& job : currentJobs) {
            for (auto task = job.pendingTask.begin(); task != job.pendingTask.end();) {
                Task& newTask = *task;
                newTask.machineIndex = UINT32_MAX;

                for (size_t i = 0; i < machine.size(); ++i) {
                    if (machine[i].IsPlaceable(newTask)) {
                        newTask.endTime = newTask.estimate != UINT64_MAX ? time + newTask.estimate / SPEEDUP_TASK_TIME : UINT64_MAX;
                        PlaceTaskOnMachine(newTask, i);
                        runningTask.push(newTask);
                        break;
                    }
                }

                if (newTask.machineIndex == UINT32_MAX) {
                    ++task;
                } else {
                    task = job.pendingTask.erase(task);
                }
            }
        }
    }

    inline void PlaceTaskOnMachine(Task& task, size_t machineIndex) {
        task.machineIndex = machineIndex;
        machine[machineIndex].PlaceTask(task);

        usedCPU += task.cpuRequest;
        usedMemory += task.memoryRequest;
        usedDisk += task.diskSpaceRequest;
    }

    inline void RemoveTaskFromMachine(const Task& task) {
        machine[task.machineIndex].RemoveTask(task);

        usedCPU -= task.cpuRequest;
        usedMemory -= task.memoryRequest;
        usedDisk -= task.diskSpaceRequest;
    }

    void DeleteFinishedJobs() {
        for (auto it = currentJobs.begin(); it != currentJobs.end();) {
            if (it->pendingTask.empty()) {
                it = currentJobs.erase(it);
            } else {
                ++it;
            }
        }
    }

    void PrintClusterStat() {
        printf("CPU: %2.2f%%  Memory: %2.2f%%  Disk: %2.2f%%  ", float(usedCPU * 100 / totalCPU), float(usedMemory * 100 / totalMemory), float(usedDisk * 100 / totalDisk));
        printf("%zu\t%zu\t%zu\n", runningTask.size(), currentJobs.size(), futureJobs.size());
    }
    
    void DumpStat() {
        float avgCPU = std::accumulate(statCPU.begin(), statCPU.end(), static_cast<long double>(0)) / statCPU.size();
        float avgMemory = std::accumulate(statMemory.begin(), statMemory.end(), static_cast<long double>(0)) / statMemory.size();
        float avgDisk = std::accumulate(statDisk.begin(), statDisk.end(), static_cast<long double>(0)) / statDisk.size();

        plt::figure_size(1200, 500);
        plt::title("Cluster resource utilization in Time");
        plt::xlabel("Time (in microseconds)");
        plt::ylabel("Resource (in percent)");
        plt::plot(statTime, statCPU, {{"label", "CPU"}});
        plt::plot(statTime, vector(statTime.size(), avgCPU), {{"label", "CPU AVG at " + to_string(avgCPU)}});
        plt::plot(statTime, statMemory, {{"label", "Memory"}});
        plt::plot(statTime, vector(statTime.size(), avgMemory), {{"label", "Memory AVG at " + to_string(avgMemory)}});
        plt::plot(statTime, statDisk, {{"label", "Disk"}});
        plt::plot(statTime, vector(statTime.size(), avgDisk), {{"label", "Disk AVG at " + to_string(avgDisk)}});
        plt::legend();
        plt::savefig("./stat/cluster_utilization_speedup_job_task_" + to_string(SPEEDUP_JOB_TIME) + "_" + to_string(SPEEDUP_TASK_TIME) + ".png");
        plt::show();
    }
};

int main() {
    Cluster cluster;
    cluster.Run();

    return 0;
}

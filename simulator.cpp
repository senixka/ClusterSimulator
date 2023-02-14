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

using namespace std;

const uint64_t SPEEDUP_TIME = 4;

class Task {
public:
    uint64_t estimate = 0;
    uint64_t endTime = 0;
    double cpuRequest = 0;
    double memoryRequest = 0;
    double diskSpaceRequest = 0;
    unsigned machineIndex = UINT32_MAX;

    friend bool operator>(const Task& lhs, const Task& rhs) {
        return lhs.estimate > rhs.estimate;
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
        jobTime /= SPEEDUP_TIME;

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

    bool isPlaceable(const Task& task) const {
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
public:
    uint64_t time = 0;
    uint64_t updateIteration = 0;
    const uint64_t scheduleEach = 100;
    priority_queue<Task, vector<Task>, greater<>> runningTask;
    list<JobMaster> futureJobs;
    list<JobMaster> currentJobs;
    vector<Machine> machine;

    Cluster(size_t machineN = 12000) {
        machine.resize(machineN);

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
        while (Update());
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
            machine[task.machineIndex].RemoveTask(task);
        } else {
            return false;
        }

        if (updateIteration % scheduleEach == 1) {
            DeleteFinishedJobs();
            PrintClusterStat();
            Schedule();
        }

        return true;
    }

    void PrintClusterStat() {
        double cpuFree = 0, memFree = 0, diskFree = 0;
        auto size = static_cast<double>(machine.size());

        for (size_t i = 0; i < machine.size(); ++i) {
            cpuFree += machine[i].cpuCapacity;
            memFree += machine[i].memoryCapacity;
            diskFree += machine[i].diskSpaceCapacity;
        }

        cout << fixed << setprecision(4) << cpuFree / size * 100 << "%\t" << memFree / size * 100 << "%\t" << diskFree / size * 100 << "%\t";
        cout << runningTask.size() << "\t" << currentJobs.size() << "\t" << futureJobs.size() << '\n';
    }

    void Schedule() {
        for (auto& job : currentJobs) {
            for (auto task = job.pendingTask.begin(); task != job.pendingTask.end();) {
                Task newTask = *task;
                newTask.machineIndex = UINT32_MAX;

                for (size_t i = 0; i < machine.size(); ++i) {
                    if (machine[i].isPlaceable(newTask)) {
                        machine[i].PlaceTask(newTask);

                        newTask.machineIndex = i;
                        newTask.endTime = newTask.estimate != UINT64_MAX ? (newTask.estimate + time) / SPEEDUP_TIME : UINT64_MAX;

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

    void DeleteFinishedJobs() {
        for (auto it = currentJobs.begin(); it != currentJobs.end();) {
            if (it->pendingTask.empty()) {
                it = currentJobs.erase(it);
            } else {
                ++it;
            }
        }
    }
};

int main() {
    Cluster cluster;
    cluster.Run();

    return 0;
}

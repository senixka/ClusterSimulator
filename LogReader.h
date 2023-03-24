#ifndef CLUSTERSIMULATOR_LOGREADER_H
#define CLUSTERSIMULATOR_LOGREADER_H

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#define COUT_OPT(X) ((X).has_value() ? (std::cout << (X).value()) : (std::cout << "NONE"))
std::ostream& operator<< (std::ostream& out, std::ostream& /*ignore*/);

enum TaskAndJobEventType {
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

enum MachineEventType {
    /*
    ● ADD (0): a machine became available to the cluster - all machines in the trace will
      have an ADD event.
    ● REMOVE (1): a machine was removed from the cluster. Removals can occur due to
      failures or maintenance.
    ● UPDATE (2): a machine available to the cluster had its available resources changed.
    */

    ADD = 0,
    REMOVE = 1,
    UPDATE = 2,
};


struct JobEvent {
    /*
    file pattern	                        field number    content	            format          mandatory
    job_events/part-?????-of-?????.csv.gz	1               time	            INTEGER         YES
    job_events/part-?????-of-?????.csv.gz	2               missing info	    INTEGER         NO
    job_events/part-?????-of-?????.csv.gz	3               job ID	            INTEGER         YES
    job_events/part-?????-of-?????.csv.gz	4               event type          INTEGER         YES
    job_events/part-?????-of-?????.csv.gz	5               user	            STRING_HASH     NO
    job_events/part-?????-of-?????.csv.gz	6               scheduling class    INTEGER         NO
    job_events/part-?????-of-?????.csv.gz	7               job name            STRING_HASH     NO
    job_events/part-?????-of-?????.csv.gz	8               logical job name    STRING_HASH     NO
    */

    uint64_t time;
    std::optional<uint64_t> missingInfo;
    uint64_t jobID;
    TaskAndJobEventType eventType;
    std::optional<std::string> user;
    std::optional<uint64_t> schedulingClass;
    std::optional<std::string> jobName;
    std::optional<std::string> logicalJobName;

    JobEvent() = default;

    JobEvent(const JobEvent&) = default;
    JobEvent& operator=(const JobEvent&) = default;

    JobEvent(JobEvent&&) = delete;
    JobEvent& operator=(JobEvent&&) = delete;

    friend std::ostream& operator<<(std::ostream& out, const JobEvent& job) {
        out << "JOB_EVENT"
            << "\tTime: " << job.time
            << "\tMissingInfo: " << COUT_OPT(job.missingInfo)
            << "\tJobID: " << job.jobID
            << "\tEventType: " << job.eventType
            << "\tUser: " << COUT_OPT(job.user)
            << "\tSchedulingClass: " << COUT_OPT(job.schedulingClass)
            << "\tJobName: " << COUT_OPT(job.jobName)
            << "\tLogicalName: " << COUT_OPT(job.logicalJobName);
        return out;
    }
};

struct TaskEvent {
    /*
    file pattern	                        field number	content	                format	      mandatory
    task_events/part-?????-of-?????.csv.gz	1               time	                INTEGER	      YES
    task_events/part-?????-of-?????.csv.gz	2               missing info	        INTEGER	      NO
    task_events/part-?????-of-?????.csv.gz	3               job ID	                INTEGER	      YES
    task_events/part-?????-of-?????.csv.gz	4               task index              INTEGER	      YES
    task_events/part-?????-of-?????.csv.gz	5               machine ID              INTEGER	      NO
    task_events/part-?????-of-?????.csv.gz	6               event type              INTEGER	      YES
    task_events/part-?????-of-?????.csv.gz	7               user	                STRING_HASH   NO
    task_events/part-?????-of-?????.csv.gz	8               scheduling class        INTEGER	      NO
    task_events/part-?????-of-?????.csv.gz	9               priority                INTEGER	      YES
    task_events/part-?????-of-?????.csv.gz	10              CPU request             FLOAT	      NO
    task_events/part-?????-of-?????.csv.gz	11              memory request          FLOAT	      NO
    task_events/part-?????-of-?????.csv.gz	12              disk space request      FLOAT	      NO
    task_events/part-?????-of-?????.csv.gz	13              machines restriction    BOOLEAN       NO
    */

    uint64_t time; // max = 18446744073709551615
    std::optional<uint64_t> missingInfo;
    uint64_t jobID;
    uint64_t taskIndex; // max = 90049
    std::optional<uint64_t> machineID;
    TaskAndJobEventType eventType;
    std::optional<std::string> user;
    std::optional<uint64_t> schedulingClass;
    uint64_t priority; // max = 11
    std::optional<long double> cpuRequest;
    std::optional<long double> memoryRequest;
    std::optional<long double> diskSpaceRequest;
    std::optional<bool> machinesRestriction;

    TaskEvent() = default;

    TaskEvent(const TaskEvent&) = default;
    TaskEvent& operator=(const TaskEvent&) = default;

    TaskEvent(TaskEvent&&) = delete;
    TaskEvent& operator=(TaskEvent&&) = delete;

    friend std::ostream& operator<<(std::ostream& out, const TaskEvent& task) {
        out << "TASK_EVENT"
            << "\tTime: " << task.time
            << "\tMissingInfo: " << COUT_OPT(task.missingInfo)
            << "\tJobID: " << task.jobID
            << "\tTaskIndex: " << task.taskIndex
            << "\tMachineID: " << COUT_OPT(task.machineID)
            << "\tEventType: " << task.eventType
            << "\tUser: " << COUT_OPT(task.user)
            << "\tSchedulingClass: " << COUT_OPT(task.schedulingClass)
            << "\tPriority: " << task.priority
            << "\tCpuRequest: " << COUT_OPT(task.cpuRequest)
            << "\tMemoryRequest: " << COUT_OPT(task.memoryRequest)
            << "\tDiskSpaceRequest: " << COUT_OPT(task.diskSpaceRequest)
            << "\tmachinesRestrictions: " << COUT_OPT(task.machinesRestriction);
        return out;
    }
};

struct MachineEvent {
    /*
    file pattern                                field number	content	        format	       mandatory
    machine_events/part-00000-of-00001.csv.gz   1               time            INTEGER        YES
    machine_events/part-00000-of-00001.csv.gz   2               machine ID      INTEGER        YES
    machine_events/part-00000-of-00001.csv.gz   3               event type      INTEGER        YES
    machine_events/part-00000-of-00001.csv.gz   4               platform ID     STRING_HASH    NO
    machine_events/part-00000-of-00001.csv.gz   5               CPUs            FLOAT          NO
    machine_events/part-00000-of-00001.csv.gz   6               Memory          FLOAT          NO
    */

    uint64_t time;
    uint64_t machineID;
    MachineEventType eventType;
    std::optional<std::string> platformID;
    std::optional<long double> cpu;
    std::optional<long double> memory;

    MachineEvent() = default;

    MachineEvent(const MachineEvent&) = default;
    MachineEvent& operator=(const MachineEvent&) = default;

    MachineEvent(MachineEvent&&) = delete;
    MachineEvent& operator=(MachineEvent&&) = delete;

    friend std::ostream& operator<<(std::ostream& out, const MachineEvent& machine) {
        out << "MACHINE_EVENT"
            << "\tTime: " << machine.time
            << "\tMachineID: " << machine.machineID
            << "\tEventType: " << machine.eventType
            << "\tPlatformID: " << COUT_OPT(machine.platformID)
            << "\tCpu: " << COUT_OPT(machine.cpu)
            << "\tMemory: " << COUT_OPT(machine.memory);
        return out;
    }
};


class LogReader {
    const std::string inputDir;

    const std::filesystem::path pathJobEvents = inputDir + "job_events";
    const std::filesystem::path pathTaskEvents = inputDir + "task_events";
    const std::filesystem::path pathMachineEvents = inputDir + "machine_events";

    std::vector<std::filesystem::path> jobEventFiles;
    std::vector<std::filesystem::path> taskEventFiles;
    std::vector<std::filesystem::path> machineEventFiles;

    std::ifstream jobFin;
    std::ifstream taskFin;
    std::ifstream machineFin;

    bool verboseFileProgress = false;

public:
    explicit LogReader(const std::string& inputLogDir);
    ~LogReader();

    bool NextJobEvent(JobEvent& jobEvent);
    bool NextTaskEvent(TaskEvent& taskLogEvent);
    bool NextMachineEvent(MachineEvent& machineEvent);

    void VerboseFileProgress(bool verbose);
};


#endif //CLUSTERSIMULATOR_LOGREADER_H

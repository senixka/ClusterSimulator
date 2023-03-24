#include "LogReader.h"


std::ostream& operator<< (std::ostream& out, std::ostream& /*ignore*/) { return out; }

LogReader::LogReader(const std::string& inputLogDir)
    : inputDir(inputLogDir) {

    for (const auto& entry : std::filesystem::directory_iterator(pathJobEvents)) {
        jobEventFiles.push_back(entry.path());
    }
    std::sort(jobEventFiles.begin(), jobEventFiles.end());

    for (const auto& entry : std::filesystem::directory_iterator(pathTaskEvents)) {
        taskEventFiles.push_back(entry.path());
    }
    std::sort(taskEventFiles.begin(), taskEventFiles.end());

    for (const auto& entry : std::filesystem::directory_iterator(pathMachineEvents)) {
        machineEventFiles.push_back(entry.path());
    }
    std::sort(machineEventFiles.begin(), machineEventFiles.end());
}

LogReader::~LogReader() = default;

bool LogReader::NextJobEvent(JobEvent& jobEvent) {
    if (jobEventFiles.empty()) return false;
    if (!jobFin.is_open()) {
        jobFin.open(jobEventFiles.front());
        if (verboseFileProgress) std::cout << jobEventFiles.front() << std::endl;
    }

    std::string line;
    std::getline(jobFin, line);
    if (line.empty()) {
        jobFin.close();
        jobEventFiles.erase(jobEventFiles.cbegin());
        return NextJobEvent(jobEvent);
    }

    std::stringstream ss(line);
    std::vector<std::string> params(8);

    std::string token;
    for (size_t i = 0; i < 8; ++i) {
        std::getline(ss, token, ',');
        params[i] = token;
    }

    jobEvent.missingInfo.reset();
    jobEvent.user.reset();
    jobEvent.schedulingClass.reset();
    jobEvent.jobName.reset();
    jobEvent.logicalJobName.reset();

    jobEvent.time = std::stoull(params[0]);
    if (!params[1].empty()) jobEvent.missingInfo = std::stoull(params[1]);
    jobEvent.jobID = std::stoull(params[2]);
    jobEvent.eventType = static_cast<TaskAndJobEventType>(std::stoi(params[3]));
    if (!params[4].empty()) jobEvent.user = params[4];
    if (!params[5].empty()) jobEvent.schedulingClass = std::stoull(params[5]);
    if (!params[6].empty()) jobEvent.jobName = params[6];
    if (!params[7].empty()) jobEvent.logicalJobName = params[7];

    return true;
}

bool LogReader::NextTaskEvent(TaskEvent& taskEvent) {
    if (taskEventFiles.empty()) return false;
    if (!taskFin.is_open()) {
        taskFin.open(taskEventFiles.front());
        if (verboseFileProgress) std::cout << taskEventFiles.front() << std::endl;
    }

    std::string line;
    std::getline(taskFin, line);
    if (line.empty()) {
        taskFin.close();
        taskEventFiles.erase(taskEventFiles.cbegin());
        return NextTaskEvent(taskEvent);
    }

    std::stringstream ss(line);
    std::vector<std::string> params(13);

    std::string token;
    for (size_t i = 0; i < 13; ++i) {
        std::getline(ss, token, ',');
        params[i] = token;
    }

    taskEvent.missingInfo.reset();
    taskEvent.machineID.reset();
    taskEvent.user.reset();
    taskEvent.schedulingClass.reset();
    taskEvent.cpuRequest.reset();
    taskEvent.memoryRequest.reset();
    taskEvent.diskSpaceRequest.reset();
    taskEvent.machinesRestriction.reset();

    taskEvent.time = std::stoull(params[0]);
    if (!params[1].empty()) taskEvent.missingInfo = std::stoull(params[1]);
    taskEvent.jobID = std::stoull(params[2]);
    taskEvent.taskIndex = std::stoull(params[3]);
    if (!params[4].empty()) taskEvent.machineID = std::stoull(params[4]);
    taskEvent.eventType = static_cast<TaskAndJobEventType>(std::stoi(params[5]));
    if (!params[6].empty()) taskEvent.user = params[6];
    if (!params[7].empty()) taskEvent.schedulingClass = std::stoull(params[7]);
    taskEvent.priority = std::stoull(params[8]);
    if (!params[9].empty()) taskEvent.cpuRequest = std::stold(params[9]);
    if (!params[10].empty()) taskEvent.memoryRequest = std::stold(params[10]);
    if (!params[11].empty()) taskEvent.diskSpaceRequest = std::stold(params[11]);
    if (!params[12].empty()) taskEvent.machinesRestriction = std::stoi(params[12]);

    return true;
}

bool LogReader::NextMachineEvent(MachineEvent& machineEvent) {
    if (machineEventFiles.empty()) return false;
    if (!machineFin.is_open()) {
        machineFin.open(machineEventFiles.front());
        if (verboseFileProgress) std::cout << machineEventFiles.front() << std::endl;
    }

    std::string line;
    std::getline(machineFin, line);
    if (line.empty()) {
        machineFin.close();
        machineEventFiles.erase(machineEventFiles.cbegin());
        return NextMachineEvent(machineEvent);
    }

    std::stringstream ss(line);
    std::vector<std::string> params(6);

    std::string token;
    for (size_t i = 0; i < 6; ++i) {
        std::getline(ss, token, ',');
        params[i] = token;
    }

    machineEvent.platformID.reset();
    machineEvent.cpu.reset();
    machineEvent.memory.reset();

    machineEvent.time = std::stoull(params[0]);
    machineEvent.machineID = std::stoull(params[1]);
    machineEvent.eventType = static_cast<MachineEventType>(std::stoi(params[2]));

    if (!params[3].empty()) machineEvent.platformID = params[3];
    if (!params[4].empty()) machineEvent.cpu = std::stold(params[4]);
    if (!params[5].empty()) machineEvent.memory = std::stold(params[5]);

    return true;
}

void LogReader::VerboseFileProgress(bool verbose) {
    verboseFileProgress = verbose;
}


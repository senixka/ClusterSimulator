#include "../simulator/Macro.h"
#include "../simulator/Registry.h"

#include <vector>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <unordered_map>


class Experiment {
public:
    Experiment(const std::string& taskAndJobFilePath, const std::string& machineFilePath, const std::string& outputFilePath,
               const std::string& jobManagerName, const std::string& taskManagerName, const std::string& placingStrategyName)
            : taskAndJobFilePath_(taskAndJobFilePath), machineFilePath_(machineFilePath), outputFilePath_(outputFilePath),
              jobManagerName_(jobManagerName), taskManagerName_(taskManagerName), placingStrategyName_(placingStrategyName) {
        ASSERT(nameToJobManagerType_.find(jobManagerName_) != nameToJobManagerType_.end());
        ASSERT(nameToTaskManagerType_.find(taskManagerName_) != nameToTaskManagerType_.end());
        ASSERT(nameToPlacingStrategyType_.find(placingStrategyName_) != nameToPlacingStrategyType_.end());
        ASSERT(!taskAndJobFilePath_.empty());
        ASSERT(!machineFilePath_.empty());
        ASSERT(!outputFilePath_.empty());
    }

public:
    const std::string taskAndJobFilePath_;
    const std::string machineFilePath_;
    const std::string outputFilePath_;
    const std::string jobManagerName_;
    const std::string taskManagerName_;
    const std::string placingStrategyName_;
};


void ReadInput(std::vector<Experiment>& experiments) {
    std::string eType;
    while (std::cin >> eType) {
        if (eType == "SINGLE") {
            std::string taskAndJobFilePath, machineFilePath, outputFilePath;
            std::string jobManagerName, taskManagerName, placingStrategyName;

            std::cin >> taskAndJobFilePath >> machineFilePath >> outputFilePath;
            std::cin >> jobManagerName >> taskManagerName >> placingStrategyName;

            experiments.push_back(Experiment(taskAndJobFilePath, machineFilePath, outputFilePath,
                                             jobManagerName, taskManagerName, placingStrategyName));
        } else if (eType == "MULTI") {
            std::vector<std::pair<std::string, std::string>> taskAndJobFilePath, machineFilePath;
            std::string outputFilePathPrefix;
            std::vector<std::string> jobManagerName, taskManagerName, placingStrategyName;

            auto ReadPairValues = [](std::vector<std::pair<std::string, std::string>>& values) {
                size_t size;
                std::cin >> size;
                values.resize(size);

                for (size_t i = 0; i < size; ++i) {
                    std::cin >> values[i].first >> values[i].second;
                }
            };

            ReadPairValues(taskAndJobFilePath);
            ReadPairValues(machineFilePath);
            std::cin >> outputFilePathPrefix;

            {
                size_t size;
                std::cin >> size;

                jobManagerName.resize(size);
                taskManagerName.resize(size);
                placingStrategyName.resize(size);

                for (size_t i = 0; i < size; ++i) {
                    std::cin >> jobManagerName[i] >> taskManagerName[i] >> placingStrategyName[i];
                }
            }

            for (const auto& [taskAndJobFile, taskAndJobMnemonic] : taskAndJobFilePath) {
                for (const auto& [machineFile, machineMnemonic] : machineFilePath) {
                    for (size_t i = 0; i < jobManagerName.size(); ++i) {
                        const std::string updatedPrefix = outputFilePathPrefix +
                                                          taskAndJobMnemonic + "_" +
                                                          machineMnemonic + "_" +
                                                          jobManagerName[i] + "_" +
                                                          taskManagerName[i] + "_" +
                                                          placingStrategyName[i] + ".txt";
                        experiments.push_back(Experiment(taskAndJobFile, machineFile,
                                                         updatedPrefix,
                                                         jobManagerName[i], taskManagerName[i], placingStrategyName[i]));
                    }
                }
            }
        } else if (eType == "CARTESIAN") {
            std::vector<std::pair<std::string, std::string>> taskAndJobFilePath, machineFilePath;
            std::string outputFilePathPrefix;
            std::vector<std::string> jobManagerName, taskManagerName, placingStrategyName;

            auto ReadValues = [](std::vector<std::string>& values) {
                size_t size;
                std::cin >> size;
                values.resize(size);

                for (size_t i = 0; i < size; ++i) {
                    std::cin >> values[i];
                }
            };

            auto ReadPairValues = [](std::vector<std::pair<std::string, std::string>>& values) {
                size_t size;
                std::cin >> size;
                values.resize(size);

                for (size_t i = 0; i < size; ++i) {
                    std::cin >> values[i].first >> values[i].second;
                }
            };

            ReadPairValues(taskAndJobFilePath);
            ReadPairValues(machineFilePath);
            std::cin >> outputFilePathPrefix;
            ReadValues(jobManagerName);
            ReadValues(taskManagerName);
            ReadValues(placingStrategyName);

            for (const auto& [taskAndJobFile, taskAndJobMnemonic] : taskAndJobFilePath) {
                for (const auto& [machineFile, machineMnemonic] : machineFilePath) {
                    for (const auto& jobManager : jobManagerName) {
                        for (const auto& taskManager : taskManagerName) {
                            for (const auto& placingStrategy : placingStrategyName) {
                                const std::string updatedPrefix = outputFilePathPrefix +
                                                                  taskAndJobMnemonic + "_" +
                                                                  machineMnemonic + "_" +
                                                                  jobManager + "_" +
                                                                  taskManager + "_" +
                                                                  placingStrategy + ".txt";
                                experiments.push_back(Experiment(taskAndJobFile, machineFile,
                                                                 updatedPrefix,
                                                                 jobManager, taskManager, placingStrategy));
                            }
                        }
                    }
                }
            }
        } else {
            UNREACHABLE("Bad experiment type");
        }
    }
}

void WriteExperimentConfig(const Experiment& experiment, const std::string& configFilePath) {
    std::ofstream fout;
    fout.open(configFilePath);

    fout << experiment.taskAndJobFilePath_ << std::endl;
    fout << experiment.machineFilePath_ << std::endl;
    fout << experiment.outputFilePath_ << std::endl;
    fout << experiment.jobManagerName_ << std::endl;
    fout << experiment.taskManagerName_ << std::endl;
    fout << experiment.placingStrategyName_ << std::endl;

    fout.close();
}

void WriteCrashReport(const std::string& message, const std::string& outputFilePath, const Experiment& experiment) {
    perror(message.c_str());

    std::ofstream fout;
    fout.open(outputFilePath);

    fout << message << std::endl;

    fout << experiment.taskAndJobFilePath_ << std::endl;
    fout << experiment.machineFilePath_ << std::endl;
    fout << experiment.outputFilePath_ << std::endl;
    fout << experiment.jobManagerName_ << std::endl;
    fout << experiment.taskManagerName_ << std::endl;
    fout << experiment.placingStrategyName_ << std::endl;

    fout.close();
}

int main() {
    std::cout << "If you do not need console output for child processes, enter 0.\n"
                 "To run child processes in separate xterm consoles, enter 1." << std::endl;

    int childMode{0};
    std::cin >> childMode;

    size_t procMax, currentWorkingProcCount{0};
    std::cin >> procMax;
    std::cout << "Using procMax = " << procMax << std::endl;

    std::vector<Experiment> experiments;
    ReadInput(experiments);

    std::cout << "Experiments count = " << experiments.size() << std::endl;

    size_t experimentIndex{0};
    while (experimentIndex < experiments.size()) {
        printf("Done %lu / %lu experiments\n", experimentIndex, experiments.size());

        if (currentWorkingProcCount < procMax) {
            const std::string configFileName = "sim_in_" + std::to_string(experimentIndex) + ".txt";

            pid_t childPID = fork();

            if (childPID == 0) {
                printf("Run child with PID %jd\n", (intmax_t) getpid());

                sleep(1);

                if (system(NULL) == 0) {
                    WriteCrashReport("No shell in child", "crash_" + configFileName, experiments[experimentIndex]);
                    _exit(EXIT_SUCCESS);
                }

                WriteExperimentConfig(experiments[experimentIndex], configFileName);
                printf("Mapping %s -> %s\n", experiments[experimentIndex].outputFilePath_.c_str(), configFileName.c_str());

                sleep(1);

                std::string command = "../../simulator/build/simulator.out < " + configFileName + " > /dev/null";
                if (childMode == 1) {
                    command =
                        "statusfile=$(mktemp);"
                        "xterm -e sh -c '../../simulator/build/simulator.out < " + configFileName + "; echo $? > '$statusfile;"
                        "status=$(cat $statusfile);"
                        "rm $statusfile;"
                        "exit $status;";
                }

                if (system(command.c_str()) != 0) {
                    WriteCrashReport("Error in system call", "crash_" + configFileName, experiments[experimentIndex]);
                    _exit(EXIT_SUCCESS);
                }

                sleep(1);

                command = "rm " + configFileName;
                if (system(command.c_str()) != 0) {
                    WriteCrashReport("Error removing sim_in file", "crash_" + configFileName, experiments[experimentIndex]);
                    _exit(EXIT_SUCCESS);
                }

                sleep(1);

                _exit(EXIT_SUCCESS);
            }

            if (childPID == -1) {
                WriteCrashReport("fork", "crash_" + configFileName, experiments[experimentIndex]);
                exit(EXIT_SUCCESS);
            }

            ++currentWorkingProcCount;
            ++experimentIndex;
        } else {
            int status{0};
            pid_t finishedPID = wait(&status);

            if (finishedPID == -1) {
                perror("waitpid");
                exit(EXIT_SUCCESS);
            }

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("Child with PID %jd finished\n", (intmax_t) finishedPID);
                --currentWorkingProcCount;
            } else {
                printf("Child with PID %jd ", (intmax_t) finishedPID);

                if (WIFEXITED(status)) {
                    printf("exited, status=%d\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("killed by signal %d\n", WTERMSIG(status));
                } else if (WIFSTOPPED(status)) {
                    printf("stopped by signal %d\n", WSTOPSIG(status));
                } else if (WIFCONTINUED(status)) {
                    printf("continued\n");
                }

                printf("Bad child return\n");
            }
        }
    }

    pid_t finishedPID;
    int status{0};

    while ((finishedPID = wait(&status)) != -1) {
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Child with PID %jd finished\n", (intmax_t) finishedPID);
            --currentWorkingProcCount;
        } else {
            printf("Child with PID %jd ", (intmax_t) finishedPID);

            if (WIFEXITED(status)) {
                printf("exited, status=%d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("killed by signal %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
            }

            printf("Bad child return\n");
        }
    }

    return 0;
}
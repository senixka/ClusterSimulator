#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <random>


long double RoundResource(long double value) {
    assert(0 <= value && value <= 1);
    return value;
}


int main() {
    const uint64_t rSeed = 179;

    std::string outputFilePath;
    std::cin >> outputFilePath;

    uint64_t jobN, jobSize, jobInterArrivalTime, taskEstimate;
    std::cin >> jobN >> jobSize >> jobInterArrivalTime >> taskEstimate;

    long double taskMeanCpuRequest, taskMeanMemoryRequest, taskMeanDiskRequest;
    std::cin >> taskMeanCpuRequest >> taskMeanMemoryRequest >> taskMeanDiskRequest;

    long double taskStdDevCpuRequest, taskStdDevMemoryRequest, taskStdDevDiskRequest;
    std::cin >> taskStdDevCpuRequest >> taskStdDevMemoryRequest >> taskStdDevDiskRequest;

    std::srand(rSeed);
    std::mt19937 gen{rSeed};

    std::normal_distribution<long double> distributionCpu{taskMeanCpuRequest, taskStdDevCpuRequest};
    std::normal_distribution<long double> distributionMemory{taskMeanMemoryRequest, taskStdDevMemoryRequest};
    std::normal_distribution<long double> distributionDisk{taskMeanDiskRequest, taskStdDevDiskRequest};


    std::ofstream fout;
    fout.open(outputFilePath);

    fout << jobN << "\n\n";

    for (size_t jobID = 0; jobID < jobN; ++jobID) {
        fout << jobID << " " << jobID * jobInterArrivalTime << "\n";
        fout << jobSize << "\n";

        const long double cpu = RoundResource(distributionCpu(gen));
        const long double memory = RoundResource(distributionMemory(gen));
        const long double disk = RoundResource(distributionDisk(gen));

        for (size_t taskIndex = 0; taskIndex < jobSize; ++taskIndex) {
            fout << taskIndex << " " << taskEstimate << " " << cpu << " " << memory << " " << disk << "\n";
        }

        fout << "\n";
    }

    fout.close();

    return 0;
}

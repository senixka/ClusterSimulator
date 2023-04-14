#include "Experiment.h"

#include <string>
#include <cstdlib>
#include <iostream>


int main(int /*argc*/, char** /*argv*/) {
    std::srand(179);

    std::string inputJobAndTaskFilePath, inputMachineFilePath, outputTxtFilePath;
    std::cin >> inputJobAndTaskFilePath >> inputMachineFilePath >> outputTxtFilePath;

    std::string jobManagerName, taskManagerName, placingStrategyName;
    std::cin >> jobManagerName >> taskManagerName >> placingStrategyName;

    Experiment experiment;
    experiment.Do(inputJobAndTaskFilePath, inputMachineFilePath, outputTxtFilePath, jobManagerName, taskManagerName, placingStrategyName);

    return 0;
}

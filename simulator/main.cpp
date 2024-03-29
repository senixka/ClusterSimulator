#include "Experiment.h"

#include <string>
#include <cstdlib>
#include <iostream>


int main() {
    std::srand(179);

    std::string inputJobAndTaskFilePath, inputMachineFilePath, outputTxtFilePath;
    std::cin >> inputJobAndTaskFilePath >> inputMachineFilePath >> outputTxtFilePath;

    std::string jobManagerName, taskManagerName, placingStrategyName;
    std::cin >> jobManagerName >> taskManagerName >> placingStrategyName;

    Experiment experiment(inputJobAndTaskFilePath, inputMachineFilePath, outputTxtFilePath, jobManagerName, taskManagerName, placingStrategyName);
    experiment.Do();

    return 0;
}

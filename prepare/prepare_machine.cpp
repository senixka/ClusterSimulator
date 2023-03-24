#include "../LogReader.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    const std::string outputFilePath = "../../simulator/input/machine.txt";
    const std::string inputLogDir = "../../google_cluster_trace_log/";

    LogReader log(inputLogDir);
    log.VerboseFileProgress(true);

    MachineEvent event;
    std::unordered_map<uint64_t, MachineEvent> machines;

    while (log.NextMachineEvent(event)) {
        if (event.eventType == MachineEventType::ADD || event.eventType == MachineEventType::UPDATE) {
            machines[event.machineID] = event;
        }
    }

    std::vector<std::pair<long double, long double>> values;

    for (const auto& [key, value] : machines) {
        if (!value.cpu.has_value() || !value.memory.has_value()) {
            continue;
        }

        values.emplace_back(value.cpu.value(), value.memory.value());
    }

    std::sort(values.begin(), values.end());


    std::ofstream out;
    out.open(outputFilePath);

    out << machines.size() << '\n';

    for (const auto& value : values) {
        out << value.first << " " << value.second << " " << 1 << '\n';
    }

    out.close();

    return 0;
}
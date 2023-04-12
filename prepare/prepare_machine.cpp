#include "../LogReader.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>
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

    std::map<std::pair<long double, long double>, size_t> values;

    for (const auto& [key, value] : machines) {
        if (!value.cpu.has_value() || !value.memory.has_value()) {
            continue;
        }

        ++values[{value.cpu.value(), value.memory.value()}];
    }

    std::ofstream out;
    out.open(outputFilePath);

    out << values.size() << '\n';

    for (const auto& [key, value] : values) {
        out << key.first << " " << key.second << " " << 0.5 << " " << value << '\n';
    }

    out.close();

    return 0;
}
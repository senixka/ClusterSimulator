#include "LogReader.h"

#include <algorithm>
#include <fstream>
#include <vector>
#include <unordered_map>


int main() {
    LogReader log;

    std::unordered_map<uint64_t, MachineEvent> machines;

    MachineEvent event;
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
    out.open("prepared_machine.txt");

    out << machines.size() << '\n';

    for (const auto& value : values) {
        out << value.first << " " << value.second << '\n';
    }

    out.close();

    return 0;
}
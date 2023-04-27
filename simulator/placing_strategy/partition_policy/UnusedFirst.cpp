#include "UnusedFirst.h"

#include "../../Macro.h"


namespace placing_strategy::partition_policy {

size_t UnusedFirst::Partition(std::vector<const Machine*>& machines) {
    ASSERT(!machines.empty());

    size_t first{0};
    const size_t last = machines.size();

    for (; first < last && machines[first]->currentTaskCount_ == 0; ++first) {
    }

    if (first == last) {
        return first;
    }

    for (size_t i = first + 1; i != last; ++i) {
        if (machines[i]->currentTaskCount_ == 0) {
            std::swap(machines[first], machines[i]);
            ++first;
        }
    }

    return first;
}

} // namespace placing_strategy::partition_policy

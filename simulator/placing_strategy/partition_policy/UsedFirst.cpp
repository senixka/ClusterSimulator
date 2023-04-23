#include "UsedFirst.h"

#include "../../Macro.h"


namespace placing_strategy::partition_policy {

unsigned UsedFirst::Partition(std::vector<const Machine*>& machines) {
    ASSERT(!machines.empty());

    unsigned first{0};
    const unsigned last = static_cast<unsigned>(machines.size());

    for (; first < last && machines[first]->currentTaskCount_ != 0; ++first) {
    }

    if (first == last) {
        return first;
    }

    for (unsigned i = first + 1; i != last; ++i) {
        if (machines[i]->currentTaskCount_ != 0) {
            std::swap(machines[first], machines[i]);
            ++first;
        }
    }

    return first;
}

} // namespace placing_strategy::partition_policy
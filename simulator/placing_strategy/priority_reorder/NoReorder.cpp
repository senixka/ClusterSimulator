#include "NoReorder.h"


namespace placing_strategy::priority_reorder {

unsigned NoReorder::Reorder(std::vector<const Machine *> &machines) const {
    return static_cast<unsigned>(machines.size());
}

} // namespace placing_strategy::priority_reorder

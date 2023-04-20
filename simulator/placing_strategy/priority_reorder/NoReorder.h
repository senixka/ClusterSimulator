#pragma once

#include "../../MachineManager.h"


namespace placing_strategy::priority_reorder {

class NoReorder {
public:
    unsigned Reorder(std::vector<const Machine*>& machines) const;
};

} // namespace placing_strategy::priority_reorder

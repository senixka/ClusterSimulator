#pragma once

#include "IPlacingStrategy.h"
#include "../Defines.h"
#include "../Macro.h"

#include <cmath>
#include <random>


namespace placing_strategy {

static_assert(static_cast<uint64_t>(MACHINE_MAX_POSSIBLE_CPU) * static_cast<uint64_t>(MACHINE_MAX_POSSIBLE_CPU) +
              static_cast<uint64_t>(MACHINE_MAX_POSSIBLE_MEMORY) * static_cast<uint64_t>(MACHINE_MAX_POSSIBLE_MEMORY) <
              static_cast<uint64_t>(UINT32_MAX));

template<class PartitionPolicy>
struct Tetris : public IPlacingStrategy {
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* task) override {
        ASSERT(!machines.empty());

        size_t split = PartitionPolicy::Partition(machines);
        if (split == 0) {
            split = machines.size();
        }

        unsigned bestMachineIndex{0};
        double minAngle{10}, currentAngle;

        const unsigned t_cpu = task->cpuRequest_;
        const unsigned t_mem = task->memoryRequest_;

        for (size_t i = 0; i < split; ++i) {
            const Machine* m = machines[i];
            const unsigned m_cpu{m->availableCpu_}, m_mem{m->availableMemory_};

            if (m_cpu * t_mem >= m_mem * t_cpu) {
                currentAngle = std::atan2(m_cpu * t_mem - m_mem * t_cpu, m_cpu * t_cpu + m_mem * t_mem);
            } else {
                currentAngle = std::atan2(m_mem * t_cpu - m_cpu * t_mem, m_cpu * t_cpu + m_mem * t_mem);
            }

            if (currentAngle < minAngle) {
                minAngle = currentAngle;
                bestMachineIndex = m->machineIndex_;
            }
        }

        return bestMachineIndex;
    }
};

} // namespace placing_strategy

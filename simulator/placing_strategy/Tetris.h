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
        double minAngle{10};

        const unsigned t_cpu = task->cpuRequest_;
        const unsigned t_mem = task->memoryRequest_;
        const double t_norm{std::sqrt(t_cpu * t_cpu + t_mem * t_mem)};

        for (size_t i = 0; i < split; ++i) {
            const Machine* m = machines[i];

            const unsigned m_cpu{m->availableCpu_}, m_mem{m->availableMemory_};

            const double dot_product = m_cpu * t_cpu + m_mem * t_mem;
            const double m_norm{std::sqrt(m_cpu * m_cpu + m_mem * m_mem)};

            const double angle = std::acos(dot_product / (m_norm * t_norm));

            if (angle < minAngle) {
                minAngle = angle;
                bestMachineIndex = m->machineIndex_;
            }
        }

        return bestMachineIndex;
    }
};

} // namespace placing_strategy

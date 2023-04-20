#pragma once

#include "IPlacingStrategy.h"
#include "../Macro.h"

#include <random>


namespace placing_strategy {

template<class PriorityReorder>
class Tetris : public IPlacingStrategy {
public:
    unsigned BestMachineIndex(std::vector<const Machine*>& machines, const Task* task) override {
        ASSERT(!machines.empty());

        unsigned split = reorder_.Reorder(machines);
        if (split == 0) {
            split = static_cast<unsigned>(machines.size());
        }

        unsigned bestMachineIndex{0};
        double minAngle{10};

        const auto t_cpu = task->cpuRequest_;
        const auto t_mem = task->memoryRequest_;
        const auto t_disk = task->diskSpaceRequest_;

        for (unsigned i = 0; i < split; ++i) {
            const Machine* machine = machines[i];

            const auto& [m_cpu, m_mem, m_disk] = machine->GetResources();

            auto dot_product = m_cpu * t_cpu + m_mem * t_mem + m_disk * t_disk;
            auto m_sq_norm = m_cpu * m_cpu + m_mem * m_mem + m_disk * m_disk;
            auto t_sq_norm = t_cpu * t_cpu + t_mem * t_mem + t_disk * t_disk;

            auto angle = std::acos(dot_product / std::sqrt(m_sq_norm * t_sq_norm));

            if (angle < minAngle) {
                minAngle = angle;
                bestMachineIndex = machine->machineIndex_;
            }
        }

        return bestMachineIndex;
    }

private:
    PriorityReorder reorder_;
};

} // namespace placing_strategy

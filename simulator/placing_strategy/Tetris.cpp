#include "Tetris.h"

#include "../Macro.h"


size_t Tetris::BestMachineIndex(const MachineManager::ReturnQueryType& machines, const Task* task) {
    ASSERT(!machines.empty());

    unsigned bestMachineIndex{0};
    double minAngle{10};

    const auto& t_cpu = task->cpuRequest_;
    const auto& t_mem = task->memoryRequest_;
    const auto& t_disk = task->diskSpaceRequest_;

    for (const auto& [machine_resources, machineIndex] : machines) {
        const auto& m_cpu = machine_resources.get<0>();
        const auto& m_mem = machine_resources.get<1>();
        const auto& m_disk = machine_resources.get<2>();

        auto dot_product = m_cpu * t_cpu + m_mem * t_mem + m_disk * t_disk;
        auto m_sq_norm = m_cpu * m_cpu + m_mem * m_mem + m_disk * m_disk;
        auto t_sq_norm = t_cpu * t_cpu + t_mem * t_mem + t_disk * t_disk;

        auto angle = std::acos(dot_product / std::sqrt(m_sq_norm * t_sq_norm));

        if (angle < minAngle) {
            minAngle = angle;
            bestMachineIndex = machineIndex;
        }
    }

    return bestMachineIndex;
}

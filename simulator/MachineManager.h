#ifndef CLUSTERSIMULATOR_MACHINEMANAGER_H
#define CLUSTERSIMULATOR_MACHINEMANAGER_H

#include "Machine.h"
#include "Task.h"

#include <vector>

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;


class MachineManager {
public:
    using point_3d = bg::model::point<double, 3, bg::cs::cartesian>;
    using box_3d = bg::model::box<point_3d>;
    using rtree_3d = bg::index::rtree<std::pair<point_3d, size_t>, bg::index::quadratic<16>>;
    using ReturnQueryType = std::vector<std::pair<point_3d, size_t>>;

private:
    std::vector<Machine> machines;
    rtree_3d tree;

public:
    MachineManager();

    void FindSuitableMachines(const Task& task, ReturnQueryType& result);

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task, size_t machineIndex);

    const std::vector<Machine>& GetAllMachines();
};


#endif //CLUSTERSIMULATOR_MACHINEMANAGER_H

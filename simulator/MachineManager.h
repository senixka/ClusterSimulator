#ifndef CLUSTERSIMULATOR_MACHINEMANAGER_H
#define CLUSTERSIMULATOR_MACHINEMANAGER_H

#include "Machine.h"
#include "Task.h"

#include <vector>

#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 3, bg::cs::cartesian> point_3d;
typedef bg::model::point<double, 2, bg::cs::cartesian> point_2d;
typedef bg::model::box<point_3d> box_3d;
typedef bg::model::box<point_2d> box_2d;
typedef bg::index::rtree<std::pair<point_3d, size_t>, bg::index::quadratic<16>> rtree_3d;
typedef bg::index::rtree<std::pair<point_2d, size_t>, bg::index::quadratic<16>> rtree_2d;


class MachineManager {
    std::vector<Machine> machines;
    rtree_3d tree;

public:
    MachineManager();

    void FindSuitableMachines(const Task& task, std::vector<std::pair<point_3d, size_t>>& result);

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task, size_t machineIndex);
    //bool IsTaskPlaceableOnMachine(const Task& task, size_t machineIndex) const;

    const std::vector<Machine>& GetAllMachines();
};


#endif //CLUSTERSIMULATOR_MACHINEMANAGER_H

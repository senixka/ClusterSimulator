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
    typedef bg::model::point<double, 3, bg::cs::cartesian> point_3d;
    typedef bg::model::box<point_3d> box_3d;
    typedef bg::index::rtree<std::pair<point_3d, size_t>, bg::index::quadratic<16>> rtree_3d;
    typedef std::vector<std::pair<point_3d, size_t>> ReturnQueryType;

private:
    std::vector<Machine> machines;
    rtree_3d tree;

public:
    MachineManager();

    void FindSuitableMachines(const Task& task, ReturnQueryType& result);

    void PlaceTaskOnMachine(Task& task, size_t machineIndex);
    void RemoveTaskFromMachine(const Task& task, size_t machineIndex);
    //bool IsTaskPlaceableOnMachine(const Task& task, size_t machineIndex) const;

    const std::vector<Machine>& GetAllMachines();
};


#endif //CLUSTERSIMULATOR_MACHINEMANAGER_H

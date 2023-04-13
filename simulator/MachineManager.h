#pragma once

#include "Machine.h"
#include "Task.h"

#include <vector>
#include <string>

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
    MachineManager(const std::string& inputFilePath);

    void FindSuitableMachines(const Task& task, ReturnQueryType& result);

    void PlaceTaskOnMachine(const Task& task);
    void RemoveTaskFromMachine(const Task& task);

    const std::vector<Machine>& GetAllMachines();
};

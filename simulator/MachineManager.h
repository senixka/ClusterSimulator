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
private:
    using point_2d = bg::model::point<unsigned, 2, bg::cs::cartesian>;
    using box_2d = bg::model::box<point_2d>;

    // https://www.boost.org/doc/libs/1_57_0/libs/geometry/doc/html/geometry/spatial_indexes/introduction.html
    // https://groups.google.com/g/boost-list/c/pPgWc2Wf2Bo
    using tree_entry = std::pair<point_2d, size_t>;
    using rtree_2d = bg::index::rtree<tree_entry, bg::index::quadratic<16>>;
    using ReturnQueryType = std::vector<tree_entry>;

public:
    explicit MachineManager(const std::string& inputFilePath);

    void FindSuitableMachines(const Task& task, std::vector<const Machine*>& result);

    void PlaceTaskOnMachine(const Task& task);
    void RemoveTaskFromMachine(const Task& task);

    const std::vector<Machine>& GetAllMachines() const;

private:
    std::vector<Machine> machines_;

    rtree_2d tree_;
    ReturnQueryType innerQueryResults_;
};

#pragma once

#include <list>
#include <limits>
#include <utility>

using vertex = size_t;
using weight = int;
using edge = std::tuple<vertex, vertex, weight>;

class Graph
{
public:
    static constexpr weight no_edge = std::numeric_limits<int>::max();
    virtual ~Graph() = default;
    virtual size_t get_vertex_count() const = 0;
    virtual weight get_weight(const vertex src, const vertex dst) const = 0;
    virtual std::list<std::pair<vertex, weight>> get_neighbors(const vertex v) const = 0;
    virtual weight set_edge(const vertex src, const vertex dst, const weight w) = 0;
    virtual weight remove_edge(const vertex src, const vertex dst) = 0;
};

#pragma once

#include <list>
#include <limits>
#include <utility>
#include <vector>

using vertex = size_t;
using weight = double;

class Graph
{
public:
    struct edge
    {
        vertex a;
        vertex b;
        weight w;
        edge(vertex a, vertex b, weight w) : a(a), b(b), w(w) {}
    };
    static constexpr weight no_edge = std::numeric_limits<weight>::max();
    virtual ~Graph() = default;
    virtual size_t get_vertex_count() const = 0;
    virtual weight get_weight(const vertex src, const vertex dst) const = 0;
    virtual std::list<std::pair<vertex, weight>> get_neighbors(const vertex v) const = 0;
    virtual weight set_edge(const vertex src, const vertex dst, const weight w) = 0;
    virtual weight set_edge(const edge e) = 0;
    virtual weight remove_edge(const vertex src, const vertex dst) = 0;
    virtual std::vector<edge> get_edges() const = 0;
};

inline bool operator<(const Graph::edge a, const Graph::edge b)
{
    return a.w < b.w;
}

inline bool operator==(const Graph::edge a, const Graph::edge b)
{
    return a.a == b.a && b.a == b.b && a.w == b.w;
}

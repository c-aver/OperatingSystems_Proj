#pragma once

#include <list>
#include <vector>
#include "Graph.hpp"

class AdjacencyGraph : public Graph
{
private:
    std::vector<std::vector<int>> adjacency_matrix;

public:
    AdjacencyGraph(size_t vertices, std::vector<std::tuple<vertex, vertex, int>> edges);
    virtual size_t get_vertex_count() const override;
    virtual int get_weight(const vertex src, const vertex dst) const override;
    virtual std::list<std::pair<vertex, weight>> get_neighbors(const vertex v) const override;
    virtual weight set_edge(const vertex src, const vertex dst, const weight w) override;
    virtual weight remove_edge(const vertex src, const vertex dst) override;

};

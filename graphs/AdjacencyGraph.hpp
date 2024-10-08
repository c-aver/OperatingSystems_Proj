#pragma once

#include <list>
#include <vector>
#include <stdexcept>
#include "Graph.hpp"

class AdjacencyGraph : public Graph
{
private:
    std::vector<std::vector<weight>> adjacency_matrix;

public:
    AdjacencyGraph(size_t vertices, std::vector<Graph::edge> edges);

    size_t get_vertex_count() const override;

    weight get_weight(const vertex src, const vertex dst) const override;

    std::list<std::pair<vertex, weight>> get_neighbors(const vertex v) const override;

    weight set_edge(const vertex src, const vertex dst, const weight w) override;

    weight set_edge(const Graph::edge e) override;

    weight remove_edge(const vertex src, const vertex dst) override;

    std::vector<Graph::edge> get_edges() const override;
};

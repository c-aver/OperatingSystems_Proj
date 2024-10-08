#pragma once

#include <vector>
#include <list>
#include <map>
#include "Graph.hpp"

class vertex_not_in_subgraph_exception
{
private:
    vertex v;
public:
    vertex_not_in_subgraph_exception(vertex v) : v(v) {}
    vertex get_vertex()
    {
        return v;
    }
};

class Subgraph : public Graph
{
private:
    Graph &super;
    std::map<vertex, std::list<vertex>> adjacency_list;

public:
    Subgraph(Graph &super) : super(super) {}
    bool has_vertex(const vertex v) const;
    std::list<vertex> get_vertices() const;
    void add_vertex(vertex v);
    void remove_vertex(vertex v);
    size_t get_vertex_count() const override;
    weight get_weight(const vertex src, const vertex dst) const override;
    std::list<std::pair<vertex, weight>> get_neighbors(const vertex v) const override;
    weight set_edge(const vertex src, const vertex dst, const weight w) override;
    weight set_edge(const Graph::edge e) override;
    void add_edge(const vertex src, const vertex dst);
    weight remove_edge(const vertex src, const vertex dst) override;
    std::vector<Graph::edge> get_edges() const override;
};
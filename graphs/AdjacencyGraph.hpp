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
    AdjacencyGraph(size_t vertices, std::vector<Graph::edge> edges)
    {
        this->adjacency_matrix.reserve(vertices);
        std::vector<weight> empty_row(vertices, Graph::no_edge);

        for (size_t i = 0; i < vertices; i++)
        {
            this->adjacency_matrix.push_back(empty_row); // Initialize all vertices to have no neighbours
        }

        // Set neighbours on the adjacency matrix
        for (auto e : edges)
        {
            if (e.a == e.b && e.w != 0)
            {
                throw std::invalid_argument("All self edges must have a weight of 0");
            }
            this->set_edge(e);
        }

        for (vertex i = 0; i < vertices; ++i)
        {
            this->set_edge(i, i, 0);
        }
    }

    size_t get_vertex_count() const override
    {
        return adjacency_matrix.size();
    }

    weight get_weight(const vertex src, const vertex dst) const override
    {
        return this->adjacency_matrix.at(src).at(dst);
    }

    std::list<std::pair<vertex, weight>> get_neighbors(const vertex v) const override
    {
        std::list<std::pair<vertex, weight>> output;
        for (size_t i = 0; i < this->get_vertex_count(); i++)
        {
            if (this->adjacency_matrix.at(v).at(i) != Graph::no_edge && i != v) // Is true == there's an edge between them
                output.push_back(std::make_pair(i, this->adjacency_matrix.at(v).at(i)));
        }
        return output;
    }

    weight set_edge(const vertex src, const vertex dst, const weight w) override
    {
        if (src >= this->get_vertex_count() || dst >= this->get_vertex_count())
            throw std::invalid_argument("Error: Can't add this edge, Invalid argument passed");
        if (src == dst && w != 0)
            throw std::invalid_argument("All self edges must have a weight of 0");

        weight output = this->adjacency_matrix.at(src).at(dst);
        this->adjacency_matrix.at(src).at(dst) = w;
        this->adjacency_matrix.at(dst).at(src) = w;
        return output;
    }

    weight set_edge(const Graph::edge e) override
    {
        return this->set_edge(e.a, e.b, e.w);
    }

    weight remove_edge(const vertex src, const vertex dst) override
    {
        if (src >= this->get_vertex_count() || dst >= this->get_vertex_count())
            throw std::invalid_argument("Error: Can't remove this edge, Invalid argument passed");
        if (src == dst)
            throw std::invalid_argument("Cannot remove self edge");

        weight output = this->adjacency_matrix.at(src).at(dst);
        this->adjacency_matrix.at(src).at(dst) = Graph::no_edge;
        this->adjacency_matrix.at(dst).at(src) = Graph::no_edge;
        return output;
    }

    std::vector<Graph::edge> get_edges() const override
    {
        std::vector<Graph::edge> result;
        for (vertex i = 0; i < this->get_vertex_count(); ++i)
        {
            for (vertex j = i + 1; j < this->get_vertex_count(); ++j)
            {
                if (adjacency_matrix[i][j] != Graph::no_edge)
                {
                    result.push_back(Graph::edge(i, j, adjacency_matrix[i][j]));
                }
            }
        }
        return result;
    }
};

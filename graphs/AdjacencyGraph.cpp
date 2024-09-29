#include "AdjacencyGraph.hpp"
#include <stdexcept>

using std::vector, std::pair, std::tuple;

AdjacencyGraph::AdjacencyGraph(size_t vertices, vector<edge> edges)
{
    this->adjacency_matrix.reserve(vertices);
    vector<weight> empty_row(vertices, Graph::no_edge);

    for (size_t i = 0; i < vertices; i++)
    {
        this->adjacency_matrix.push_back(empty_row); // Initialize all vertices to have no neighbours
    }

    // Set neighbours on the adjacency matrix
    for (auto p : edges)
    {
        if (std::get<0>(p) == std::get<1>(p) && std::get<2>(p) != 0)
        {
            throw std::invalid_argument("All self edges must have a weight of 0");
        }
        std::apply([this](auto &&... args) { this->set_edge(args...); }, p);
    }

    for (vertex i = 0; i < vertices; ++i)
    {
        this->set_edge(i, i, 0);
    }
}

size_t AdjacencyGraph::get_vertex_count() const
{
    return adjacency_matrix.size();
}

weight AdjacencyGraph::get_weight(const vertex src, const vertex dst) const
{
    return this->adjacency_matrix.at(src).at(dst);
}

std::list<std::pair<vertex, weight>> AdjacencyGraph::get_neighbors(const vertex v) const
{
    std::list<std::pair<vertex, weight>> output;
    for (size_t i = 0; i < this->get_vertex_count(); i++)
    {
        if (this->adjacency_matrix.at(v).at(i) != no_edge && i != v) // Is true == there's an edge between them
            output.push_back(std::make_pair(i, this->adjacency_matrix.at(v).at(i)));
    }
    return output;
}

weight AdjacencyGraph::set_edge(const vertex src, const vertex dst, const weight w)
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

weight AdjacencyGraph::remove_edge(const vertex src, const vertex dst)
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

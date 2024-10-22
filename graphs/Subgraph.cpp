#include <set>
#include <vector>
#include <algorithm>
#include <list>
#include <stdexcept>

#include "Graph.hpp"
#include "Subgraph.hpp"

size_t Subgraph::get_vertex_count() const
{
    return this->adjacency_list.size();
}

std::list<vertex> Subgraph::get_vertices() const
{
    std::list<vertex> result;
    for (auto &[v, _] : this->adjacency_list)
    {
        result.push_back(v);
    }
    return result;
}

bool Subgraph::has_vertex(const vertex v) const
{
    return this->adjacency_list.find(v) != this->adjacency_list.end();
}

void Subgraph::add_vertex(vertex v)
{
    using std::operator""s;
    if (v >= super.get_vertex_count())
        throw std::invalid_argument("Vertex "s + std::to_string(v) + " does not exist in supergraph"s);
    this->adjacency_list[v] = std::list<vertex>();
}

void Subgraph::remove_vertex(vertex v)
{
    using std::operator""s;
    if (v >= super.get_vertex_count())
        throw std::invalid_argument("Vertex "s + std::to_string(v) + " does not exist in supergraph"s);
    this->adjacency_list.erase(v);
}

weight Subgraph::get_weight(const vertex src, const vertex dst) const
{
    if (!has_vertex(src) || !has_vertex(dst))
        throw vertex_not_in_subgraph_exception(!has_vertex(src) ? dst : src);
    std::list<vertex> neighbors = adjacency_list.find(src)->second;
    if (std::find(neighbors.begin(), neighbors.end(), dst) == neighbors.end())
        return Graph::no_edge;
    return super.get_weight(src, dst);
}

bool Subgraph::has_edge(const vertex src, const vertex dst) const
{
    if (!has_vertex(src) || !has_vertex(dst))
        throw vertex_not_in_subgraph_exception(!has_vertex(src) ? dst : src);
    std::list<vertex> neighbors = adjacency_list.find(src)->second;
    return std::find(neighbors.begin(), neighbors.end(), dst) != neighbors.end();
}

std::list<std::pair<vertex, weight>> Subgraph::get_neighbors(const vertex v) const
{
    if (!has_vertex(v))
        throw vertex_not_in_subgraph_exception(v);
    std::list<std::pair<vertex, weight>> result;
    for (vertex u : this->adjacency_list.at(v))
    {
        weight w = super.get_weight(v, u);
        if (w != Graph::no_edge)
            result.push_back(std::make_pair(u, w));
    }
    return result;
}

weight Subgraph::set_edge(const vertex src, const vertex dst, const weight u)
{
    (void)src;
    (void)dst;
    (void)u;
    throw std::runtime_error("set_edge is not supported in Subgraph");
}

weight Subgraph::set_edge(const Graph::edge e)
{
    (void)e;
    throw std::runtime_error("set_edge is not supported in Subgraph");
}

void Subgraph::add_edge(const vertex src, const vertex dst)
{
    if (!has_vertex(src) || !has_vertex(dst))
        throw vertex_not_in_subgraph_exception(!has_vertex(src) ? dst : src);
    if (std::find(this->adjacency_list.at(src).begin(), this->adjacency_list.at(src).end(), dst) != this->adjacency_list.at(src).end())
        return;
    this->adjacency_list.at(src).push_back(dst);
    this->adjacency_list.at(dst).push_back(src);
}

weight Subgraph::remove_edge(const vertex src, const vertex dst)
{
    if (!has_vertex(src) || !has_vertex(dst))
        throw vertex_not_in_subgraph_exception(!has_vertex(src) ? dst : src);
    auto it = std::find(this->adjacency_list.at(src).begin(), this->adjacency_list.at(src).end(), dst);
    weight output = it == this->adjacency_list.at(src).end() ? Graph::no_edge : super.get_weight(src, dst);
    this->adjacency_list.at(src).remove(dst);
    this->adjacency_list.at(dst).remove(src);
    return output;
}

std::vector<Graph::edge> Subgraph::get_edges() const
{
    std::vector<Graph::edge> result;
    for (vertex v : this->get_vertices())
    {
        for (auto &[u, w] : this->get_neighbors(v))
        {
            if (u < w)
                result.push_back(Graph::edge(v, u, w));
        }
    }
    return result;
}

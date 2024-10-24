#include <vector>
#include <list>
#include <utility>
#include <queue>
#include <algorithm>

#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
#include "Prim.hpp"

struct prim_vertex
{
    vertex v;
    weight cost;
    vertex parent;
    
    bool operator==(prim_vertex u)
    {
        return u.v == this->v && u.cost == this->cost && u.parent == this->parent;
    }
};

/**
 * @brief Pop the vertex with the minimum cost from the costs vector
 * @param costs Vector of costs
 * @return Vertex with the minimum cost
 */
prim_vertex pop_min(std::vector<prim_vertex> &costs)
{
    // Find the element with the minimum cost
    auto min_cost_it = std::min_element(costs.begin(), costs.end(),
                                        [](const prim_vertex &a, const prim_vertex &b)
                                        {
                                            return a.cost < b.cost;
                                        });

    // Extract the minimum cost element
    prim_vertex min_cost_pv = *min_cost_it;

    // Remove the minimum element from the vector
    costs.erase(min_cost_it);
    return min_cost_pv;
}

/**
 * @brief Modify the costs of the neighbours of the vertex pv
 * @param g Graph
 * @param costs Vector of costs
 * @param pv Vertex to modify the neighbours of
 */
void modify_neighbours(Graph *g, std::vector<prim_vertex> &costs, prim_vertex pv)
{
    std::list<std::pair<vertex, weight>> neighbours = g->get_neighbors(pv.v);
    for (const auto &neighbour : neighbours)
    {
        auto &neighbour_vertex = neighbour.first;
        auto weight = neighbour.second;

        // Find the corresponding vertex in the costs vector
        auto costs_iter = std::find_if(costs.begin(), costs.end(),
                                       [&](const prim_vertex &c)
                                       { return c.v == neighbour_vertex; });

        // If the neighbour is found and the current edge weight is less than the stored cost
        if (costs_iter != costs.end() && costs_iter->cost > weight)
        {
            costs_iter->cost = weight;
            costs_iter->parent = pv.v;
        }
    }
}

/**
 * @brief Prim's algorithm to find the minimum spanning tree of a graph
 * @param g Graph
 * @return Subgraph containing the minimum spanning tree
 */
Subgraph prim(Graph *g)
{
    std::vector<prim_vertex> costs;
    for (vertex v = 0; v < g->get_vertex_count(); v++)
    {
        // Initialize costs vector with each vertex cost as infinity and parent as max vertex
        costs.push_back(prim_vertex{v, std::numeric_limits<weight>::infinity(), std::numeric_limits<vertex>::max()});
    }
    Subgraph result(*g);                    // Create a subgraph from the original graph (Initially empty)
    prim_vertex pv = pop_min(costs);        // Get the vertex with the minimum cost as the start vertex
    result.add_vertex(pv.v);                // Add the start vertex to the subgraph
    modify_neighbours(g, costs, pv);        // Modify the costs of the neighbours of the start vertex
    while (!costs.empty())                  // While not all vertices are added to the subgraph
    {
        prim_vertex pv = pop_min(costs);    // Get the vertex with the minimum cost
        result.add_vertex(pv.v);            
        result.add_edge(pv.v, pv.parent);   
        modify_neighbours(g, costs, pv);    
    }
    return result;
}
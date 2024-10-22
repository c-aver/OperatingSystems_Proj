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
    inline bool operator<(prim_vertex u)
    {
        return u.cost > this->cost;
    }

    bool operator==(prim_vertex u)
    {
        return u.v == this->v && u.cost == this->cost && u.parent == this->parent;
    }
};

prim_vertex pop_min(std::vector<prim_vertex> &costs)
{
    prim_vertex min_cost_pv = costs.front();
    for (prim_vertex pv : costs)
    {
        if (pv.cost < min_cost_pv.cost)
        {
            min_cost_pv = pv;
        }
    }
    costs.erase(std::find(costs.begin(), costs.end(), min_cost_pv));
    return min_cost_pv;
}

void modify_neighboors(Graph *g, std::vector<prim_vertex> *costs, prim_vertex pv)
{
    std::list<std::pair<vertex, weight>> neighboors = g->get_neighbors(pv.v);
    for (auto neighboors_it = neighboors.begin(); neighboors_it != neighboors.end(); neighboors_it++)
    {
        for (auto costs_it = costs->begin(); costs_it != costs->end(); costs_it++)
        {
            if (neighboors_it->first == costs_it->v && costs_it->cost > g->get_weight(neighboors_it->first, pv.v))
            {
                costs_it->cost = g->get_weight(neighboors_it->first, pv.v);
                costs_it->parent = pv.v;
            }
        }
    }
}

Subgraph prim(Graph *g)
{

    std::vector<prim_vertex> costs;
    for (vertex v = 0; v < g->get_vertex_count(); v++)
    {
        costs.push_back(prim_vertex{v, std::numeric_limits<weight>::infinity(), std::numeric_limits<vertex>::max()});
    }
    Subgraph result(*g);
    prim_vertex pv = pop_min(costs);
    result.add_vertex(pv.v);
    modify_neighboors(g, &costs, pv);
    while (!costs.empty())
    {
        prim_vertex pv = pop_min(costs);
        result.add_vertex(pv.v);
        result.add_edge(pv.v, pv.parent);
        modify_neighboors(g, &costs, pv);
    }
    return result;
}
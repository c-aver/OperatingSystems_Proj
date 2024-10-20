#include <vector>
#include <list>
#include <utility>
#include <queue>

#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
#include "Prim.hpp"

Subgraph prim(Graph *g)
{
    struct prim_vertex
    {
        vertex v;
        weight cost;
        vertex parent;
        inline bool operator<(prim_vertex u)
        {
            return u.cost > this->cost;
        }
    };

    std::vector<prim_vertex> costs;
    for (vertex v = 0; v < g->get_vertex_count(); v++)
    {
        costs.push_back(prim_vertex{v, std::numeric_limits<weight>::infinity(), std::numeric_limits<vertex>::max()});
    }
    Subgraph result(*g);
    std::priority_queue<prim_vertex> cost_priority_q(std::less<prim_vertex>(), costs);
    for (vertex v = 0; v < g->get_vertex_count(); v++)
    {
        cost_priority_q.push(v);
    }
    while (!cost_priority_q.empty())
    {
        prim_vertex pv = cost_priority_q.top();
        result.add_vertex(pv.v);
        result.add_edge(pv.v, pv.parent);
        std::list<std::pair<vertex, weight>> neighboors = g->get_neighbors(pv.v);
        for (auto neighboors_it = neighboors.begin(); neighboors_it != neighboors.end(); neighboors_it++)
        {
            for (auto cpq_it = cost_priority_q.begin(); costs_it != costs.end(); costs_it++)        //TODO!!
            {
                if (neighboors_it->first() == costs_it->v && costs_it->cost > g->get_weight(neighboors_it->first, pv.v))
                {
                    costs_it->cost = g->get_weight(neighboors_it->first, pv.v);
                    costs_it->parent = pv.v;
                }
            }
        }
        cost_priority_q.pop();
        
    }
}
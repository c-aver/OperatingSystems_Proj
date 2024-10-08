#pragma once

#include <functional>
#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
using mst_strat_t = Subgraph(Graph*);
#include "Kruskal.hpp"


mst_strat_t *mst_strategy() // TODO: parameters
{
    mst_strat_t *result = &kruskal;
    return result;
};

#pragma once

#include <string>
#include <functional>
#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
using mst_strat_t = Subgraph(Graph*);
#include "Kruskal.hpp"


mst_strat_t *mst_strategy(std::string algo_name); // TODO: parameters

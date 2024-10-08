#pragma once

#include <numeric>
#include <algorithm>
#include <list>
#include <utility>
#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
#include "../graphs/AdjacencyGraph.hpp"

double total_weight(Subgraph *g);

/**
 * @brief Find the longest path in a graph between two vertices
 *  There are 4 cases to consider:
 *  1. The longest path is from the start vertex to one of its descendants.
 *  2. The longest path goes through the start vertex.
 *  3. The longest path is from one of the start vertex's descendants to another descendant, but doesn't go through the start vertex.
 *  4. The longest path is only between the start vertex and itself.
 */
double longest_distance_between_two_vertice(Subgraph *g);

double average_distance_between_two_vertices(Subgraph *g);

double shortest_distance_between_two_vertices(Subgraph *g);


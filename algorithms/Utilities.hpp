#pragma once

#include <numeric>
#include <algorithm>
#include <list>
#include <utility>
#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
#include "../graphs/AdjacencyGraph.hpp"

/**
 * @brief Find the total weight of the MST
 * @param g The MST
 * @return The total weight of the MST
 */
double total_weight(Subgraph *g);

/**
 * @brief Find the longest path in a graph between two vertices
 *  There are 4 cases to consider:
 *  1. The longest path is from the start vertex to one of its descendants.
 *  2. The longest path goes through the start vertex.
 *  3. The longest path is from one of the start vertex's descendants to another descendant, but doesn't go through the start vertex.
 *  4. The longest path is only between the start vertex and itself.
 */
double longest_distance_between_two_vertices(Subgraph *g);

/**
 * @brief Find the average distance between two vertices in an MST
 * @param g The MST
 * @return The average distance between two vertices in the MST
 */
double average_distance_between_two_vertices(Subgraph *g);

/**
 * @brief Find the shortest path in an MST between two vertices
 * @param g The MST
 * @return The weight of the shortest path between two vertices in the MST
 */
double shortest_distance_between_two_vertices(Subgraph *g);

/**
 * @brief Check if a graph is connected
 * @param g The graph
 * @return True if the graph is connected, false otherwise
 */
bool is_connected(Graph *g);


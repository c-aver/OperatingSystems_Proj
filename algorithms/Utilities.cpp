#include <functional>
#include <list>
#include "Utilities.hpp"

using std::list, std::vector;

/*
 * This function performs a Depth First Search (DFS) on the graph to check connactivity.
 * This function is used to check if the graph is strongly connected (1 SCC).
 */
// void DFS(Graph* g, vertex current, vertex parent, vector<bool> &visited)
// {
//     visited[current] = true;

//     // Iterate over all of the current's neighbours
//     for (auto neighbor : g->get_neighbors(current))
//     {
//         if (!visited[neighbor.first] && g->get_weight(neighbor.first, current) != 0)
//         {
//             DFS(g, neighbor.first, current, visited);
//         }
//     }
// }

// /*
//  * This function checks if the graph is strongly connected.
//  * In order to check if the graph is strongly connected we run DFS on each vertex and check if all of the vertices in the graph were visited.
//  * @return True if the graph is strongly connected, False otherwise.
//  */
// bool is_connected(Graph *g)
// {
//     if (g->get_vertex_count() <= 1)
//     {
// #ifdef DEBUG
//         std::cout << "Empty graph is considered connected" << std::endl;
// #endif
//         return true;
//     }

//     // Travarse all of the vertices in the graph and run DFS on each vertex
//     size_t vertex_count = g->get_vertex_count();
//     for (size_t i = 0; i < vertex_count; i++)
//     {
//         std::vector<bool> visited(vertex_count, false); // Create a visited array for the graph nodes. initialized to false.
//         std::vector<bool> inStack(vertex_count, false); // Create a stack array for the graph nodes. initialized to false.

//         // DFS(g, i, INFINITY, visited);

//         if (std::find(visited.begin(), visited.end(), false) != visited.end()) // If there is a vertex which wasn't visited
//         {
//             return false;
//         }
//     }
//     return true; // The graph is connected if all nodes are visited
// }

double total_weight(Subgraph *g)
{
    double result = 0;
    for (Graph::edge e : g->get_edges())
    {
        result += e.w;
    }
    return result;
}

/**
 * @brief Find the longest path in a graph between two vertices
 * Is used for the longest_distance_between_two_vertices function
 * @return The weight of the longest path from the start vertex to one of its descendants
 */
double find_longest_path(Subgraph *g, vertex start, vertex parent)
{
    std::list<std::pair<vertex, weight>> neighbours = g->get_neighbors(start);
    if (neighbours.empty())
        return 0;
    auto max_element = *std::max_element(neighbours.begin(), neighbours.end(),
                                         [](const std::pair<vertex, weight> &a, const std::pair<vertex, weight> &b)
                                         { return a.second < b.second; });
    if (max_element.second < 0)
    {
        return 0;
    }

    double max = 0;
    for (auto n : neighbours)
    {
        if (n.first == parent)
            continue;
        double distance = n.second + find_longest_path(g, n.first, start);
        if (distance > max)
            max = distance;
    }
    return max;
}

double longest_distance_between_two_vertices(Subgraph *g, vertex start, vertex parent)
{
    // We will check 4 cases:

    // 1. The longest path is from the start vertex to itself (The MST is a single vertex)
    if (start == parent || start >= g->get_vertex_count())
        return 0;

    // 2. The longest path is from the start vertex to one of its descendants.
    double path_from_start = find_longest_path(g, start, parent);

    // 3. The longest path goes through the start vertex.
    double path_from_start_with_childs = 0;
    std::list<std::pair<vertex, weight>> neighbours = g->get_neighbors(start);
    if (neighbours.size() > 1)
    {
        neighbours.sort([](const std::pair<vertex, weight> &a, const std::pair<vertex, weight> &b)
                        { return a.second > b.second; });
        path_from_start_with_childs = neighbours.front().second + find_longest_path(g, neighbours.front().first, start);
        neighbours.pop_front();
        path_from_start_with_childs += neighbours.front().second + find_longest_path(g, neighbours.front().first, start);
    }
    else if (neighbours.size() == 1)
    {
        path_from_start_with_childs = neighbours.front().second + find_longest_path(g, neighbours.front().first, start);
    }

    // 4. The longest path is from one of the start vertex's descendants to another descendant, but doesn't go through the start vertex.
    double path_from_childs = 0;
    for (auto v : g->get_neighbors(start))
    {
        double path = longest_distance_between_two_vertices(g, v.first, start);
        if (path > path_from_childs)
            path_from_childs = path;
    }

    return std::max({path_from_start, path_from_childs, path_from_start_with_childs, 0.0});
}

/**
 * @brief Find the longest path in a graph between two vertices
 *  There are 4 cases to consider:
 *  1. The longest path is from the start vertex to one of its descendants.
 *  2. The longest path goes through the start vertex.
 *  3. The longest path is from one of the start vertex's descendants to another descendant, but doesn't go through the start vertex.
 *  4. The longest path is only between the start vertex and itself.
 */
double longest_distance_between_two_vertices(Subgraph *g)
{
    // We will check 4 cases:

    // 1. The longest path is from the start vertex to one of its descendants.
    vertex start = g->get_vertices().front();                                        // Start from the first vertex in the MST
    double path_from_start = find_longest_path(g, start, g->get_vertex_count() + 1); // start vx has no parent (g->get_vertex_count() + 1)

    // 2. The longest path goes through the start vertex.
    double path_from_start_with_childs = 0;
    std::list<std::pair<vertex, weight>> neighbours = g->get_neighbors(start);
    if (neighbours.size() > 1)
    {
        // Sort the neighbours by weight in descending order
        neighbours.sort([](const std::pair<vertex, weight> &a, const std::pair<vertex, weight> &b)
                        { return a.second > b.second; });
        // Get the two neighbours with the highest weights
        path_from_start_with_childs = neighbours.front().second + find_longest_path(g, neighbours.front().first, start);
        neighbours.pop_front();
        path_from_start_with_childs += neighbours.front().second + find_longest_path(g, neighbours.front().first, start);
    }
    else if (neighbours.size() == 1) // If there's only one neighbour
    {
        path_from_start_with_childs = neighbours.front().second + find_longest_path(g, neighbours.front().first, start);
    }

    // 3. The longest path is from one of the start vertex's descendants to another descendant, but doesn't go through the start vertex.
    double path_from_childs = 0;
    for (auto v : g->get_neighbors(start))
    {
        double path = longest_distance_between_two_vertices(g, v.first, start);
        if (path > path_from_childs)
            path_from_childs = path;
    }

    return std::max({path_from_start, path_from_childs, path_from_start_with_childs, 0.0});
}

double average_distance_between_two_vertices(Subgraph *g)
{
    // The idea: find how many paths go through each edge. Each edge splits the graph into two parts.
    // The number of paths that go through an edge is the number of vertices in one part times the number of vertices in the other part.

    std::map<vertex, size_t> descendents;

    // A recursive lambda function that sets the number of descendents of each vertex
    std::function<size_t(vertex, vertex)> set_descendents = [&descendents, &set_descendents, &g](vertex v, vertex parent)
    {
        descendents[v] = 1;
        for (auto n : g->get_neighbors(v))
        {
            if (n.first == parent)
                continue;
            descendents[v] += set_descendents(n.first, v); // Add the number of descendents of the child
        }
        return descendents[v];
    };

    set_descendents(g->get_vertices().front(), g->get_vertex_count()); // Start from the first vertex. which has no parent.

    double total_weight = 0.0;
    for (auto &[u, v, w] : g->get_edges())
    {
        size_t m = std::min(descendents[u], descendents[v]); // Get the minimum number of descendents that can go through this edge
        total_weight += m * (g->get_vertex_count() - m) * w; // The amount of paths that go through this edge is m * (n - m)
    }
    return total_weight / (g->get_vertex_count() * (g->get_vertex_count() - 1)); // The average distance is the total weight divided by the total number of paths
}

double shortest_distance_between_two_vertices(Subgraph *g)
{
    return std::min_element(g->get_edges().begin(), g->get_edges().end())->w; // The shortest distance is the weight of the shortest edge
}

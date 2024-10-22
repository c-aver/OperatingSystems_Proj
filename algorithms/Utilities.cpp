#include <functional>
#include <list>
#include "Utilities.hpp"

using std::list, std::vector;

/*
 * This function performs a Depth First Search (DFS) on the graph to check connactivity.
 * This function is used to check if the graph is strongly connected (1 SCC).
 */
void DFS(Graph *g, vertex current, vector<bool> &visited)
{
    visited[current] = true;

    // Iterate over all of the current's neighbours
    for (std::pair neighbor : g->get_neighbors(current))
    {
        if (!visited[neighbor.first])
        {
            DFS(g, neighbor.first, visited);
        }
    }
}

bool is_connected(Graph *g)
{
    if (g->get_vertex_count() <= 1) // Empty and single vertex graphs are considered connected
    {
        return true;
    }

    // Travarse all of the vertices in the graph and run DFS on each vertex
    size_t vertex_count = g->get_vertex_count();
    std::vector<bool> visited(vertex_count, false); // Create a visited array for the graph nodes. initialized to false.

    DFS(g, 0, visited);

    if (std::find(visited.begin(), visited.end(), false) != visited.end()) // If there is a vertex which wasn't visited
    {
        return false;
    }
    return true; // The graph is connected if all nodes are visited
}

double total_weight(Subgraph *g)
{
    double result = 0;
    for (Graph::edge e : g->get_edges())
    {
        result += e.w;
    }
    return result;
}

std::pair<double, double> longest_and_farthest(Subgraph *g, vertex v, vertex parent)
{
    double farthest = 0.0;
    double second_farthest = 0.0;
    double child_longest = 0.0;
    for (auto &[u, w] : g->get_neighbors(v))
    {
        if (u == parent)
            continue;
        auto [u_longest, u_farthest] = longest_and_farthest(g, u, v);
        u_farthest += w;
        if (u_longest > child_longest)
            child_longest = u_longest;

        // Find the two farthest vertices from the current vertex (farthest and second farthest)
        if (u_farthest > farthest)
        {
            second_farthest = farthest;
            farthest = u_farthest;
        }
        else if (u_farthest > second_farthest)
        {
            second_farthest = u_farthest;
        }
    }

    double longest = std::max({farthest + second_farthest, child_longest});
    return std::make_pair(longest, farthest);
}

double longest_distance_between_two_vertices(Subgraph *g)
{
    return longest_and_farthest(g, 0, g->get_vertex_count()).first;
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
    return total_weight / ((g->get_vertex_count() * (g->get_vertex_count() - 1)/2) +g->get_vertex_count()) ; // The average distance is the total weight divided by the total number of paths
}

double shortest_distance_between_two_vertices(Subgraph *g)
{
    double min_edge = std::numeric_limits<weight>::max();
    for (Graph::edge e : g->get_edges())
    {
        if (e.w < min_edge)
            min_edge = e.w;
    }
    return min_edge;
}

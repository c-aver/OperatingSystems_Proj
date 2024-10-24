#include <functional>
#include <list>
#include "Utilities.hpp"

using std::list, std::vector;

/*
 * This function performs a Depth First Search (DFS) on the graph to check connactivity.
 */
void DFS(Graph *g, vertex current, vector<bool> &visited)
{
    visited[current] = true; // Mark the current node as visited

    // Iterate over all of the current's neighbours
    for (std::pair neighbor : g->get_neighbors(current))
    {
        if (!visited[neighbor.first]) // If the neighbour wasn't visited yet, visit it
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

    if (std::find(visited.begin(), visited.end(), false) != visited.end()) // If there is a vertex which wasn't visited by the DFS, the graph is not connected
    {
        return false;
    }
    return true;    // The graph is connected if all nodes were visited
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
    // Initialize all distances to 0, anything below that is irrelevant
    double farthest = 0.0; // The farthest descendant from me
    double second_farthest = 0.0; // The second fathest
    // A path between them is the longest path that goes through me
    double child_longest = 0.0; // The longest path known by a child
    for (auto &[u, w] : g->get_neighbors(v))    // Iterate over all of the neighbours of the current vertex
    {
        if (u == parent)
            continue;
        auto [u_longest, u_farthest] = longest_and_farthest(g, u, v);   // Recursively find the longest path from the current vertex to its descendants
        u_farthest += w; // The farthest from me through u is the farthest from u plus the weight of u--v
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
    // The longest path I know is either one my child knew (child_longest) or a path between my two farthest descendants
    double longest = std::max({farthest + second_farthest, child_longest});
    return std::make_pair(longest, farthest);
}

double longest_distance_between_two_vertices(Subgraph *g)
{
    return longest_and_farthest(g, 0, g->get_vertex_count()).first; // 0 is an arbitrary root, first is the longest of longest_and_fathest return pair
}

double average_distance_between_two_vertices(Subgraph *g)
{
    // The idea: find how many paths go through each edge. Each edge splits the graph into two parts.
    // The number of paths that go through an edge is the number of vertices in one part times the number of vertices in the other part.
    std::map<vertex, size_t> descendents; // Remember how many descendants each vertex has (when rooted arbitrarily)

    // A recursive lambda function that sets the number of descendents of each vertex
    std::function<size_t(vertex, vertex)> set_descendents = [&descendents, &set_descendents, &g](vertex v, vertex parent)
    {
        // The first "descendant" is the vertex itself
        descendents[v] = 1;
        for (auto n : g->get_neighbors(v)) // Iterate the neighbors
        {
            if (n.first == parent) // Ignore the parent
                continue;
            // Add to my descendants the descendants of my child
            descendents[v] += set_descendents(n.first, v); // Add the number of descendents of the child
        }
        // Return to my parent the number of my descendants
        return descendents[v];
    };

    set_descendents(g->get_vertices().front(), g->get_vertex_count()); // Start from the first vertex. which has no parent.

    size_t n = g->get_vertex_count();
    double total_weight = 0.0;
    for (auto &[u, v, w] : g->get_edges())
    {
        size_t m = std::min(descendents[u], descendents[v]); // Get the minimum number of descendents that can go through this edge
        total_weight += m * (n - m) * w; // The amount of paths that go through this edge is m * (n - m)
    }
    return total_weight / ((n * (n - 1) / 2) + n); // The average distance is the total weight divided by the total number of paths, +n for paths from vertex to itself
}

double shortest_distance_between_two_vertices(Subgraph *g)
{
    if(g->get_vertex_count() <= 1)
        return 0;
    
    double min_edge = std::numeric_limits<weight>::max(); 
    for (Graph::edge e : g->get_edges())
    {
        if (e.w < min_edge)
            min_edge = e.w;
    }
    return min_edge;
}

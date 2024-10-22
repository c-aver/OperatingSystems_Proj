#include <set>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstddef>

#include "../graphs/Graph.hpp"
#include "../graphs/Subgraph.hpp"
#include "Kruskal.hpp"

Subgraph kruskal(Graph *g)
{
    Subgraph result(*g);                               // Create a subgraph from the original graph (initialized as an empty graph)
    for (vertex v = 0; v < g->get_vertex_count(); ++v) // The subgraph has the same vertices as the original graph
        result.add_vertex(v);
    std::size_t count = g->get_vertex_count();

    // union-find data structure "members"
    std::vector<vertex> parent(count);
    std::vector<std::size_t> size(count);

    // union-find "constructor" - Set each vertex as its own parent and set the size of each set to 1
    for (vertex v = 0; v < count; ++v)
    {
        parent[v] = v;
        size[v] = 1;
    }

    // union-find "methods"
    // Recursively find the parent a given vertex belongs to
    std::function<vertex(vertex)> find_set = [&parent, &size, &find_set](vertex v)  
    {
        if (v == parent[v])         // If the vertex is its own parent (Reached the root of the union-find tree)
            return v;
        return parent[v] = find_set(parent[v]); // Recursively find the parent of the vertex
    };

    // Merge the sets of two vertices
    std::function<void(vertex, vertex)> union_sets = [&parent, &size, &find_set](vertex a, vertex b)
    {
        a = find_set(a);                // Find the parent of a
        b = find_set(b);                // Find the parent of b
        if (a != b)                     // If the parents are not the same (the vertices are not on the same tree), merge the sets
        {
            if (size[a] < size[b]) 
            {
                parent[a] = b;          
                size[b] += size[a];     
            }
            else
            {
                parent[b] = a;
                size[a] += size[b];
            }
        }
    };

    std::vector<Graph::edge> edges = g->get_edges();
    std::sort(edges.begin(), edges.end());  // Sort the edges in non-decreasing order of weight - so that the smallest edge is first
    for (auto &e : edges)                   // Traverse all of the edges
    {
        auto &[a, b, _] = e;                // Get the vertices of the edge
        if (find_set(a) != find_set(b))     // If the vertices are not in the same set
        {
            result.add_edge(a, b);          // Add the edge (a,b) to the subgraph
            union_sets(a, b);               
        }
    }
    return result;
}

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
    Subgraph result(*g);
    for (vertex v = 0; v < g->get_vertex_count(); ++v)
        result.add_vertex(v);
    std::size_t count = g->get_vertex_count();
    // union-find data structure "members"
    std::vector<vertex> parent(count);
    std::vector<std::size_t> size(count);
    // union-find "constructor"
    for (vertex v = 0; v < count; ++v)
    {
        parent[v] = v;
        size[v] = 1;
    }
    // union-find "methods"
    std::function<vertex(vertex)> find_set = [&parent, &size, &find_set](vertex v)
    {
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]);
    };
    std::function<void(vertex, vertex)> union_sets = [&parent, &size, &find_set](vertex a, vertex b)
    {
        a = find_set(a);
        b = find_set(b);
        if (a != b)
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

    std::vector<Graph::edge>
        edges = g->get_edges();
    std::sort(edges.begin(), edges.end());
    for (auto &e : edges)
    {
        auto &[a, b, _] = e;
        if (find_set(a) != find_set(b))
        {
            result.add_edge(a, b);
            union_sets(a, b);
        }
    }
    return result;
}

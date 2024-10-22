#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Utilities.hpp"
#include "doctest.h"
#include "../graphs/Subgraph.hpp"

TEST_CASE("Test - total_weight")
{
    size_t vertices = 4;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {0, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {2, 3, 1};
    edges.push_back(e3);
    AdjacencyGraph g(vertices, edges);
    Subgraph mst(g);
    for(vertex v=0;v<vertices;v++)
    {
        mst.add_vertex(v);
    }
    mst.add_edge(0, 1);
    mst.add_edge(0, 2);
    mst.add_edge(2, 3);
    double x = total_weight(&mst);
    CHECK(x == 3);
}
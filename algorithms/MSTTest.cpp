#pragma once
#include <vector>
#include "Prim.hpp"
#include "doctest.h"

#include "../graphs/Graph.hpp"
#include "../graphs/AdjacencyGraph.hpp"
#include "../graphs/Subgraph.hpp"

TEST_CASE("check prim's algorithm")
{
    size_t vertices = 3;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {0, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {2, 1, 3};
    edges.push_back(e3);
    AdjacencyGraph g(vertices, edges);
    Subgraph prim_mst = prim(&g);
    std::list<vertex> mst_vs = prim_mst.get_vertices();
    for (vertex v = 0; v < vertices; v++)
    {
        CHECK(std::find(mst_vs.begin(), mst_vs.end(), v) != mst_vs.end());
    }
    std::vector<Graph::edge> mst_es = prim_mst.get_edges();
    CHECK(std::find(mst_es.begin(), mst_es.end(), e1) != mst_es.end());
    CHECK(std::find(mst_es.begin(), mst_es.end(), e2) != mst_es.end());
    CHECK(std::find(mst_es.begin(), mst_es.end(), e3) == mst_es.end());
}

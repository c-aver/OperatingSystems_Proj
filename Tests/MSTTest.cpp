#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <vector>
#include "../algorithms/Prim.hpp"
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
    CHECK(prim_mst.has_edge(0, 1));
    CHECK(prim_mst.has_edge(0, 2));
    CHECK(!prim_mst.has_edge(2, 1));
}

TEST_CASE("check prim's algorithm")
{
    size_t vertices = 6;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 2};
    edges.push_back(e1);
    Graph::edge e2 = {0, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {0, 4, 2};
    edges.push_back(e3);
    Graph::edge e4 = {2, 1, 1};
    edges.push_back(e4);
    Graph::edge e5 = {4, 2, 1};
    edges.push_back(e5);
    Graph::edge e6 = {4, 5, 2};
    edges.push_back(e6);
        Graph::edge e7 = {2, 5, 1};
    edges.push_back(e7);
    Graph::edge e8 = {1, 3, 2};
    edges.push_back(e8);
    Graph::edge e9 = {3, 5, 2};
    edges.push_back(e9);
    Graph::edge e10 = {2, 3, 1};
    edges.push_back(e10);
    AdjacencyGraph g(vertices, edges);
    Subgraph prim_mst = prim(&g);
    std::list<vertex> mst_vs = prim_mst.get_vertices();
    for (vertex v = 0; v < vertices; v++)
    {
        CHECK(std::find(mst_vs.begin(), mst_vs.end(), v) != mst_vs.end());
    }
    std::vector<Graph::edge> mst_es = prim_mst.get_edges();
    CHECK(prim_mst.has_edge(0, 2));
    CHECK(prim_mst.has_edge(1, 2));
    CHECK(prim_mst.has_edge(3, 2));
    CHECK(prim_mst.has_edge(4, 2));
    CHECK(prim_mst.has_edge(5, 2));
    CHECK(!prim_mst.has_edge(3, 1));
    CHECK(!prim_mst.has_edge(0, 1));
    CHECK(!prim_mst.has_edge(0, 4));
    CHECK(!prim_mst.has_edge(4, 5));
    CHECK(!prim_mst.has_edge(3, 5));
}

TEST_CASE("check prim's algorithm")
{
    size_t vertices = 8;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {0, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {1, 2, 2};
    edges.push_back(e3);
    Graph::edge e4 = {2, 3, 2};
    edges.push_back(e4);
    Graph::edge e5 = {4, 1, 3};
    edges.push_back(e5);
    Graph::edge e6 = {1, 5, 3};
    edges.push_back(e6);
    Graph::edge e7 = {2, 6, 3};
    edges.push_back(e7);
    Graph::edge e8 = {2, 7, 3};
    edges.push_back(e8);
    AdjacencyGraph g(vertices, edges);
    Subgraph prim_mst = prim(&g);
    std::list<vertex> mst_vs = prim_mst.get_vertices();
    for (vertex v = 0; v < vertices; v++)
    {
        CHECK(std::find(mst_vs.begin(), mst_vs.end(), v) != mst_vs.end());
    }
    std::vector<Graph::edge> mst_es = prim_mst.get_edges();
    CHECK(prim_mst.has_edge(0, 2));
    CHECK(prim_mst.has_edge(1, 0));
    CHECK(prim_mst.has_edge(3, 2));
    CHECK(prim_mst.has_edge(4, 1));
    CHECK(prim_mst.has_edge(5, 1));
    CHECK(prim_mst.has_edge(6, 2));
    CHECK(prim_mst.has_edge(7, 2));
    CHECK(!prim_mst.has_edge(2, 1));
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("check kruskal's algorithm")
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
    Subgraph kruskal_mst = kruskal(&g);
    std::list<vertex> mst_vs = kruskal_mst.get_vertices();
    for (vertex v = 0; v < vertices; v++)
    {
        CHECK(std::find(mst_vs.begin(), mst_vs.end(), v) != mst_vs.end());
    }
    std::vector<Graph::edge> mst_es = kruskal_mst.get_edges();
    CHECK(kruskal_mst.has_edge(0, 1));
    CHECK(kruskal_mst.has_edge(0, 2));
    CHECK(!kruskal_mst.has_edge(2, 1));
}

TEST_CASE("check kruskal's algorithm")
{
    size_t vertices = 6;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 2};
    edges.push_back(e1);
    Graph::edge e2 = {0, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {0, 4, 2};
    edges.push_back(e3);
    Graph::edge e4 = {2, 1, 1};
    edges.push_back(e4);
    Graph::edge e5 = {4, 2, 1};
    edges.push_back(e5);
    Graph::edge e6 = {4, 5, 2};
    edges.push_back(e6);
        Graph::edge e7 = {2, 5, 1};
    edges.push_back(e7);
    Graph::edge e8 = {1, 3, 2};
    edges.push_back(e8);
    Graph::edge e9 = {3, 5, 2};
    edges.push_back(e9);
    Graph::edge e10 = {2, 3, 1};
    edges.push_back(e10);
    AdjacencyGraph g(vertices, edges);
    Subgraph kruskal_mst = kruskal(&g);
    std::list<vertex> mst_vs = kruskal_mst.get_vertices();
    for (vertex v = 0; v < vertices; v++)
    {
        CHECK(std::find(mst_vs.begin(), mst_vs.end(), v) != mst_vs.end());
    }
    std::vector<Graph::edge> mst_es = kruskal_mst.get_edges();
    CHECK(kruskal_mst.has_edge(0, 2));
    CHECK(kruskal_mst.has_edge(1, 2));
    CHECK(kruskal_mst.has_edge(3, 2));
    CHECK(kruskal_mst.has_edge(4, 2));
    CHECK(kruskal_mst.has_edge(5, 2));
    CHECK(!kruskal_mst.has_edge(3, 1));
    CHECK(!kruskal_mst.has_edge(0, 1));
    CHECK(!kruskal_mst.has_edge(0, 4));
    CHECK(!kruskal_mst.has_edge(4, 5));
    CHECK(!kruskal_mst.has_edge(3, 5));
}

TEST_CASE("check kruskal's algorithm")
{
    size_t vertices = 8;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {0, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {1, 2, 2};
    edges.push_back(e3);
    Graph::edge e4 = {2, 3, 2};
    edges.push_back(e4);
    Graph::edge e5 = {4, 1, 3};
    edges.push_back(e5);
    Graph::edge e6 = {1, 5, 3};
    edges.push_back(e6);
    Graph::edge e7 = {2, 6, 3};
    edges.push_back(e7);
    Graph::edge e8 = {2, 7, 3};
    edges.push_back(e8);
    AdjacencyGraph g(vertices, edges);
    Subgraph kruskal_mst = kruskal(&g);
    std::list<vertex> mst_vs = kruskal_mst.get_vertices();
    for (vertex v = 0; v < vertices; v++)
    {
        CHECK(std::find(mst_vs.begin(), mst_vs.end(), v) != mst_vs.end());
    }
    std::vector<Graph::edge> mst_es = kruskal_mst.get_edges();
    CHECK(kruskal_mst.has_edge(0, 2));
    CHECK(kruskal_mst.has_edge(1, 0));
    CHECK(kruskal_mst.has_edge(3, 2));
    CHECK(kruskal_mst.has_edge(4, 1));
    CHECK(kruskal_mst.has_edge(5, 1));
    CHECK(kruskal_mst.has_edge(6, 2));
    CHECK(kruskal_mst.has_edge(7, 2));
    CHECK(!kruskal_mst.has_edge(2, 1));
}
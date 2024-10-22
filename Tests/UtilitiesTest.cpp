#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../algorithms/Utilities.hpp"
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
    double total_w = total_weight(&mst);
    CHECK(total_w == 3);
}

TEST_CASE("Test - longest_distance_between_two_vertices")
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
    Subgraph mst(g);
    for(vertex v=0;v<vertices;v++)
    {
        mst.add_vertex(v);
    }
    mst.add_edge(0, 1);
    mst.add_edge(0, 2);
    mst.add_edge(0, 4);
    mst.add_edge(2, 1);
    mst.add_edge(2, 5);
    mst.add_edge(2, 3);
    mst.add_edge(4, 5);
    mst.add_edge(1, 3);
    mst.add_edge(3, 5);
    double longest_distance = longest_distance_between_two_vertices(&mst);
    CHECK(longest_distance == 2);
}
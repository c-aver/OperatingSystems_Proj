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
    for (vertex v = 0; v < vertices; v++)
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
    size_t vertices = 4;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {1, 2, 1};
    edges.push_back(e2);
    Graph::edge e3 = {2, 3, 1};
    edges.push_back(e3);
    AdjacencyGraph g(vertices, edges);
    Subgraph mst(g);
    for (vertex v = 0; v < vertices; v++)
    {
        mst.add_vertex(v);
    }
    mst.add_edge(0, 1);
    mst.add_edge(1, 2);
    mst.add_edge(2, 3);
    double longest_distance = longest_distance_between_two_vertices(&mst);
    CHECK(longest_distance == 3);
}

TEST_CASE("Test - longest_distance_between_two_vertices with more complicated graph")
{
    size_t vertices = 5;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {2, 4, 2};
    edges.push_back(e1);
    Graph::edge e2 = {2, 3, 1};
    edges.push_back(e2);
    Graph::edge e3 = {2, 0, 1};
    edges.push_back(e3);
    Graph::edge e4 = {0, 1, 1};
    edges.push_back(e4);
    AdjacencyGraph g(vertices, edges);
    Subgraph mst(g);
    for (vertex v = 0; v < vertices; v++)
    {
        mst.add_vertex(v);
    }
    mst.add_edge(4, 2);
    mst.add_edge(2, 0);
    mst.add_edge(2, 3);
    mst.add_edge(0, 1);
    double longest_distance = longest_distance_between_two_vertices(&mst);
    CHECK(longest_distance == 4);
}

TEST_CASE("Test - average_distance_between_two_vertices")
{
    size_t vertices = 4;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {1, 2, 2};
    edges.push_back(e2);
    Graph::edge e3 = {2, 3, 1};
    edges.push_back(e3);
    AdjacencyGraph g(vertices, edges);
    Subgraph mst(g);
    for (vertex v = 0; v < vertices; v++)
    {
        mst.add_vertex(v);
    }
    mst.add_edge(0, 1);
    mst.add_edge(1, 2);
    mst.add_edge(2, 3);
    double average_distance = average_distance_between_two_vertices(&mst);
    CHECK(average_distance == (14.0 / 10.0));
}

TEST_CASE("Test - shortest_distance_between_two_vertices")
{
    size_t vertices = 4;
    std::vector<Graph::edge> edges;
    Graph::edge e1 = {0, 1, 1};
    edges.push_back(e1);
    Graph::edge e2 = {1, 2, 2};
    edges.push_back(e2);
    Graph::edge e3 = {2, 3, 1};
    edges.push_back(e3);
    AdjacencyGraph g(vertices, edges);
    Subgraph mst(g);
    for (vertex v = 0; v < vertices; v++)
    {
        mst.add_vertex(v);
    }
    mst.add_edge(0, 1);
    mst.add_edge(1, 2);
    mst.add_edge(2, 3);
    double shortest_distance = shortest_distance_between_two_vertices(&mst);
    CHECK(shortest_distance == 1);

    
}

TEST_CASE("Test - is_connected")
{
    SUBCASE("Is connected")
    {
        size_t vertices = 4;
        std::vector<Graph::edge> edges;
        Graph::edge e1 = {0, 1, 1};
        edges.push_back(e1);
        Graph::edge e2 = {1, 2, 2};
        edges.push_back(e2);
        Graph::edge e3 = {2, 3, 1};
        edges.push_back(e3);
        AdjacencyGraph g(vertices, edges);
        CHECK(is_connected(&g));
    }
    SUBCASE("Is not connected")
    {
        size_t vertices = 4;
        std::vector<Graph::edge> edges;
        Graph::edge e1 = {0, 1, 1};
        edges.push_back(e1);
        Graph::edge e2 = {2, 3, 2};
        edges.push_back(e2);
        AdjacencyGraph g(vertices, edges);
        CHECK(!is_connected(&g));
    }
}
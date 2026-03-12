#include "../include/Algorithms.h"
#include <gtest/gtest.h>
#include <limits>

class AlgorithmsTest : public ::testing::Test {
};

// --- Dijkstra Tests ---

TEST_F(AlgorithmsTest, Dijkstra_DirectedWeightedGraph)
{
    Graph g(5, true);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(3, 1, 3);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 2, 6);

    auto [dist, pred] = dijkstra(g, 0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 8);  // 0->3->1
    EXPECT_EQ(dist[2], 9);  // 0->3->1->2
    EXPECT_EQ(dist[3], 5);  // 0->3
    EXPECT_EQ(dist[4], 7);  // 0->3->4
}

TEST_F(AlgorithmsTest, Dijkstra_UnreachableVertices)
{
    Graph g(4, true);
    g.addEdge(0, 1, 5);
    // Vertices 2 and 3 are unreachable from 0.

    auto [dist, pred] = dijkstra(g, 0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 5);
    EXPECT_EQ(dist[2], std::numeric_limits<int>::max());
    EXPECT_EQ(dist[3], std::numeric_limits<int>::max());
    EXPECT_EQ(pred[2], -1);
    EXPECT_EQ(pred[3], -1);
}

TEST_F(AlgorithmsTest, Dijkstra_SingleVertex)
{
    Graph g(1, true);

    auto [dist, pred] = dijkstra(g, 0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(pred[0], -1);
}

TEST_F(AlgorithmsTest, Dijkstra_SourceOutOfRange)
{
    Graph g(3, true);
    g.addEdge(0, 1, 5);

    EXPECT_THROW(dijkstra(g, 5), std::out_of_range);
}

TEST_F(AlgorithmsTest, Dijkstra_PredecessorChain)
{
    Graph g(4, true);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 3);

    auto [dist, pred] = dijkstra(g, 0);

    EXPECT_EQ(dist[3], 6);
    EXPECT_EQ(pred[3], 2);
    EXPECT_EQ(pred[2], 1);
    EXPECT_EQ(pred[1], 0);
    EXPECT_EQ(pred[0], -1);
}

// --- Bellman-Ford Tests ---

TEST_F(AlgorithmsTest, BellmanFord_DirectedWeightedGraph)
{
    Graph g(5, true);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(3, 1, 3);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 4, 2);
    g.addEdge(4, 2, 6);

    auto [dist, pred] = bellmanFord(g, 0);

    // Should produce the same results as Dijkstra for non-negative weights.
    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 8);  // 0->3->1
    EXPECT_EQ(dist[2], 9);  // 0->3->1->2
    EXPECT_EQ(dist[3], 5);  // 0->3
    EXPECT_EQ(dist[4], 7);  // 0->3->4
}

TEST_F(AlgorithmsTest, BellmanFord_SingleVertex)
{
    Graph g(1, true);

    auto [dist, pred] = bellmanFord(g, 0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(pred[0], -1);
}

TEST_F(AlgorithmsTest, BellmanFord_DisconnectedGraph)
{
    Graph g(4, true);
    g.addEdge(0, 1, 3);
    // Vertices 2 and 3 are unreachable from 0.

    auto [dist, pred] = bellmanFord(g, 0);

    EXPECT_EQ(dist[0], 0);
    EXPECT_EQ(dist[1], 3);
    EXPECT_EQ(dist[2], std::numeric_limits<int>::max());
    EXPECT_EQ(dist[3], std::numeric_limits<int>::max());
}

TEST_F(AlgorithmsTest, BellmanFord_MatchesDijkstra)
{
    // Bellman-Ford and Dijkstra should produce identical results on non-negative graphs.
    Graph g(6, true);
    g.addEdge(0, 1, 7);
    g.addEdge(0, 2, 9);
    g.addEdge(0, 5, 14);
    g.addEdge(1, 2, 10);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 11);
    g.addEdge(2, 5, 2);
    g.addEdge(3, 4, 6);
    g.addEdge(4, 5, 9);

    auto [distD, predD] = dijkstra(g, 0);
    auto [distBF, predBF] = bellmanFord(g, 0);

    for (size_t i = 0; i < 6; ++i)
        EXPECT_EQ(distD[i], distBF[i]);
}

TEST_F(AlgorithmsTest, BellmanFord_SourceOutOfRange)
{
    Graph g(3, true);
    EXPECT_THROW(bellmanFord(g, 10), std::out_of_range);
}

// --- Topological Sort Tests ---

TEST_F(AlgorithmsTest, TopologicalSort_DAG)
{
    Graph g(6, false);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);

    std::vector<int> result = topologicalSort(g);

    EXPECT_EQ(result.size(), 6u);

    // Verify topological ordering: for each edge u->v, u appears before v.
    std::vector<int> position(6);
    for (size_t i = 0; i < result.size(); ++i)
        position[static_cast<size_t>(result[i])] = static_cast<int>(i);

    EXPECT_LT(position[5], position[2]);
    EXPECT_LT(position[5], position[0]);
    EXPECT_LT(position[4], position[0]);
    EXPECT_LT(position[4], position[1]);
    EXPECT_LT(position[2], position[3]);
    EXPECT_LT(position[3], position[1]);
}

TEST_F(AlgorithmsTest, TopologicalSort_CycleThrows)
{
    Graph g(3, false);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    EXPECT_THROW(topologicalSort(g), std::runtime_error);
}

TEST_F(AlgorithmsTest, TopologicalSort_SingleVertex)
{
    Graph g(1, false);

    std::vector<int> result = topologicalSort(g);

    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], 0);
}

TEST_F(AlgorithmsTest, TopologicalSort_DisconnectedDAG)
{
    Graph g(4, false);
    g.addEdge(0, 1);
    g.addEdge(2, 3);
    // Two disconnected components: 0->1, 2->3

    std::vector<int> result = topologicalSort(g);

    EXPECT_EQ(result.size(), 4u);

    std::vector<int> position(4);
    for (size_t i = 0; i < result.size(); ++i)
        position[static_cast<size_t>(result[i])] = static_cast<int>(i);

    EXPECT_LT(position[0], position[1]);
    EXPECT_LT(position[2], position[3]);
}

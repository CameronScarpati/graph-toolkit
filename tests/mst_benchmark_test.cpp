#include "../include/Graph.h"
#include <chrono>
#include <gtest/gtest.h>
#include <iomanip>
#include <random>

class MSTBenchmarkTest : public ::testing::Test {
protected:
    Graph createConnectedGraph(
        size_t numVertices, double extraEdgeProbability, bool weighted = true)
    {
        Graph g(numVertices, weighted);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> edgeDist(0.0, 1.0);
        std::uniform_int_distribution<> weightDist(1, 100);

        // First, ensure the graph is connected by creating a path through all vertices
        for (size_t i = 0; i < numVertices - 1; ++i) {
            int weight = weighted ? weightDist(gen) : 1;
            g.addUndirectedEdge(i, i + 1, weight);
        }

        // Add extra edges with a probability
        for (size_t i = 0; i < numVertices; ++i) {
            for (size_t j = i + 2; j < numVertices; ++j) {
                if (edgeDist(gen) < extraEdgeProbability) {
                    int weight = weighted ? weightDist(gen) : 1;
                    g.addUndirectedEdge(i, j, weight);
                }
            }
        }
        return g;
    }

    int countEdges(const Graph& g)
    {
        int edgeCount = 0;
        for (size_t i = 0; i < g.getNumVertices(); ++i)
            edgeCount += g.getNeighbors(i).size();
        return edgeCount / 2; // undirected, each edge counted twice
    }
};

TEST_F(MSTBenchmarkTest, Benchmark_50Vertices)
{
    Graph g = createConnectedGraph(50, 0.4);
    ASSERT_TRUE(g.isConnected());

    auto start = std::chrono::high_resolution_clock::now();
    Graph mst = g.minimumSpanningTree();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "[BENCHMARK] MST 50 vertices: " << duration.count() << " us" << std::endl;

    // MST must have exactly V-1 edges
    EXPECT_EQ(countEdges(mst), 49);
}

TEST_F(MSTBenchmarkTest, Benchmark_100Vertices)
{
    Graph g = createConnectedGraph(100, 0.3);
    ASSERT_TRUE(g.isConnected());

    auto start = std::chrono::high_resolution_clock::now();
    Graph mst = g.minimumSpanningTree();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "[BENCHMARK] MST 100 vertices: " << duration.count() << " us" << std::endl;

    EXPECT_EQ(countEdges(mst), 99);
}

TEST_F(MSTBenchmarkTest, Benchmark_100Vertices_Dense)
{
    Graph g = createConnectedGraph(100, 0.7);
    ASSERT_TRUE(g.isConnected());

    auto start = std::chrono::high_resolution_clock::now();
    Graph mst = g.minimumSpanningTree();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "[BENCHMARK] MST 100 vertices (dense): " << duration.count() << " us" << std::endl;

    EXPECT_EQ(countEdges(mst), 99);
}

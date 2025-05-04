#include "../include/Graph.h"
#include <chrono>
#include <gtest/gtest.h>
#include <iomanip>
#include <random>

// The fixture for testing the Graph Class.
class GraphTest : public ::testing::Test {
protected:
    // Helper method to create a random graph for stress testing
    Graph createRandomGraph(size_t numVertices, double edgeProbability, bool weighted = true)
    {
        Graph g(numVertices, weighted);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> edgeDist(0.0, 1.0);
        std::uniform_int_distribution<> weightDist(1, 100);

        for (size_t i = 0; i < numVertices; ++i) {
            for (size_t j = 0; j < numVertices; ++j) {
                if (i != j && edgeDist(gen) < edgeProbability) {
                    if (weighted) {
                        g.addEdge(i, j, weightDist(gen));
                    } else {
                        g.addEdge(i, j);
                    }
                }
            }
        }
        return g;
    }

    // Helper method to create a connected graph for MST testing
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
            for (size_t j = i + 2; j < numVertices; ++j) { // Skip adjacent vertices
                // which are already connected
                if (edgeDist(gen) < extraEdgeProbability) {
                    int weight = weighted ? weightDist(gen) : 1;
                    g.addUndirectedEdge(i, j, weight);
                }
            }
        }
        return g;
    }

    // Helper method to create a complete graph for TSP testing
    Graph createCompleteGraph(size_t numVertices, bool weighted = true)
    {
        Graph g(numVertices, weighted);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> weightDist(1, 100);

        for (size_t i = 0; i < numVertices; ++i) {
            for (size_t j = 0; j < numVertices; ++j) {
                if (i != j) {
                    int weight = weighted ? weightDist(gen) : 1;
                    g.addEdge(i, j, weight);
                }
            }
        }
        return g;
    }

    // Helper method to measure and print execution time
    template <typename Func> void measureExecutionTime(const std::string& algorithmName, Func func)
    {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "\n"
                  << algorithmName << " execution time: " << std::fixed << std::setprecision(3)
                  << static_cast<double>(duration.count()) / 1000.0 << " ms\n";
    }

    // Helper method to calculate total weight of MST
    int calculateMSTWeight(const Graph& mst)
    {
        int totalWeight = 0;
        for (size_t u = 0; u < mst.getNumVertices(); ++u) {
            for (int v : mst.getNeighbors(u)) {
                if (u < static_cast<size_t>(v)) { // Count each edge only once
                    totalWeight += mst.getEdgeWeight(u, v);
                }
            }
        }
        return totalWeight;
    }
};

// Original tests remain here...

TEST_F(GraphTest, BasicConstructor)
{
    EXPECT_NO_THROW({
        Graph g;
        EXPECT_EQ(g.getNumVertices(), 0);
        EXPECT_FALSE(g.getIsWeighted());
    });

    EXPECT_NO_THROW({
        Graph g(5);
        EXPECT_EQ(g.getNumVertices(), 5);
        EXPECT_FALSE(g.getIsWeighted());
    });

    EXPECT_NO_THROW({
        Graph g(7, true);
        EXPECT_EQ(g.getNumVertices(), 7);
        EXPECT_TRUE(g.getIsWeighted());
    });
}

TEST_F(GraphTest, CopyConstructor)
{
    Graph g1(4);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);

    Graph g2(g1);
    EXPECT_EQ(g2.getNumVertices(), 4);
    EXPECT_FALSE(g2.getIsWeighted());
    EXPECT_TRUE(g2.isAdjacent(0, 1));
    EXPECT_TRUE(g2.isAdjacent(1, 2));
    EXPECT_TRUE(g2.isAdjacent(2, 3));
    EXPECT_FALSE(g2.isAdjacent(0, 2));
}

// More original tests...

// Modified test case for MST with timing and logging
TEST_F(GraphTest, MinimumSpanningTree_Enhanced)
{
    Graph g(4, true); // weighted, undirected graph
    g.addUndirectedEdge(0, 1, 1);
    g.addUndirectedEdge(1, 2, 2);
    g.addUndirectedEdge(2, 3, 3);
    g.addUndirectedEdge(0, 3, 4);
    g.addUndirectedEdge(1, 3, 5);

    std::cout << "\n----- Minimum Spanning Tree Test -----\n";
    std::cout << "Original Graph:\n" << g.toString() << std::endl;

    Graph mst;
    measureExecutionTime("MST Algorithm", [&]() { mst = g.minimumSpanningTree(); });

    std::cout << "MST Graph:\n" << mst.toString() << std::endl;

    EXPECT_EQ(mst.getNumVertices(), 4);
    EXPECT_TRUE(mst.getIsWeighted());

    // MST should have exactly V-1 = 3 edges
    int edgeCount = 0;
    for (size_t i = 0; i < mst.getNumVertices(); ++i)
        edgeCount += mst.getNeighbors(i).size();
    EXPECT_EQ(edgeCount / 2, 3); // undirected, so each edge counted twice

    // The total weight of MST should be 1.0 + 2.0 + 3.0 = 6.0
    int totalWeight = calculateMSTWeight(mst);
    std::cout << "Total MST weight: " << totalWeight << std::endl;
    EXPECT_DOUBLE_EQ(totalWeight, 6);
}

// New stress test for MST with larger graph
TEST_F(GraphTest, MinimumSpanningTree_StressTest)
{
    const size_t numVertices = 12;
    Graph g = createConnectedGraph(numVertices, 0.3);

    std::cout << "\n----- MST Stress Test (" << numVertices << " vertices) -----\n";
    std::cout << "Graph structure:\n" << g.toString() << std::endl;
    std::cout << "Number of vertices: " << g.getNumVertices() << std::endl;

    // Count edges
    int edgeCount = 0;
    for (size_t i = 0; i < g.getNumVertices(); ++i)
        edgeCount += g.getNeighbors(i).size();
    edgeCount /= 2; // For undirected graph
    std::cout << "Number of edges: " << edgeCount << std::endl;

    Graph mst;
    measureExecutionTime("MST Algorithm", [&]() { mst = g.minimumSpanningTree(); });

    // Show MST structure
    std::cout << "MST structure:\n" << mst.toString() << std::endl;

    // Verify MST properties
    int mstEdgeCount = 0;
    for (size_t i = 0; i < mst.getNumVertices(); ++i)
        mstEdgeCount += mst.getNeighbors(i).size();
    mstEdgeCount /= 2; // For undirected graph

    int totalWeight = calculateMSTWeight(mst);

    std::cout << "MST edges: " << mstEdgeCount << std::endl;
    std::cout << "Total MST weight: " << totalWeight << std::endl;

    EXPECT_EQ(mstEdgeCount, numVertices - 1);
}

// Modified test for TSP with timing and logging
TEST_F(GraphTest, TravelingSalesman_Enhanced)
{
    Graph g(4, true); // weighted graph
    g.addUndirectedEdge(0, 1, 10);
    g.addUndirectedEdge(1, 2, 15);
    g.addUndirectedEdge(2, 3, 20);
    g.addUndirectedEdge(3, 0, 25);
    g.addUndirectedEdge(0, 2, 35);
    g.addUndirectedEdge(1, 3, 30);

    std::cout << "\n----- Traveling Salesman Test -----\n";
    std::cout << "Graph:\n" << g.toString() << std::endl;

    std::pair<std::vector<int>, int> tspResult;
    measureExecutionTime("TSP Algorithm", [&]() { tspResult = g.travelingSalesman(); });

    std::vector<int> path = tspResult.first;
    int cost = tspResult.second;

    std::cout << "TSP path: ";
    for (int vertex : path) {
        std::cout << vertex << " ";
    }
    std::cout << "\nTSP cost: " << cost << std::endl;

    // Path should be a cycle (start == end), and visit all vertices once
    EXPECT_EQ(path.front(), path.back());
    EXPECT_EQ(path.size(), g.getNumVertices() + 1);
    EXPECT_EQ(cost, 70);
}

// New stress test for TSP with larger graph
TEST_F(GraphTest, TravelingSalesman_StressTest)
{
    const size_t numVertices = 10; // TSP complexity grows factorially, so 10 is already challenging
    Graph g = createCompleteGraph(numVertices);

    std::cout << "\n----- TSP Stress Test (" << numVertices << " vertices) -----\n";
    std::cout << "Complete graph with " << numVertices << " vertices\n";
    std::cout << "Graph structure:\n" << g.toString() << std::endl;

    std::pair<std::vector<int>, int> tspResult;
    measureExecutionTime("TSP Algorithm", [&]() { tspResult = g.travelingSalesman(); });

    std::vector<int> path = tspResult.first;
    int cost = tspResult.second;

    std::cout << "TSP path: ";
    for (int vertex : path) {
        std::cout << vertex << " ";
    }
    std::cout << "\nTSP cost: " << cost << std::endl;

    // Verify properties
    EXPECT_EQ(path.front(), path.back());
    EXPECT_EQ(path.size(), g.getNumVertices() + 1);
}

// Modified test for Hamiltonian Cycles with timing and logging
TEST_F(GraphTest, FindHamiltonianCycles_Enhanced)
{
    Graph g(5, false);
    // Create a cycle 0-1-2-3-4-0
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 0);
    // Add some extra edges
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    std::cout << "\n----- Hamiltonian Cycles Test -----\n";
    std::cout << "Graph:\n" << g.toString() << std::endl;

    std::vector<std::vector<int>> cycles;
    measureExecutionTime(
        "Hamiltonian Cycles Algorithm", [&]() { cycles = g.findHamiltonianCycles(); });

    std::cout << "Found " << cycles.size() << " Hamiltonian cycles\n";

    if (!cycles.empty()) {
        std::cout << "First cycle: ";
        for (int vertex : cycles[0]) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }

    // Verify cycles
    for (const auto& cycle : cycles) {
        EXPECT_EQ(cycle.size(), g.getNumVertices() + 1);
        EXPECT_EQ(cycle.front(), cycle.back());
    }
}

// New stress test for Hamiltonian Cycles
TEST_F(GraphTest, FindHamiltonianCycles_StressTest)
{
    const size_t numVertices = 10; // Complexity grows factorially
    // Create a graph that's likely to have Hamiltonian cycles
    // (high-edge density)
    Graph g = createRandomGraph(numVertices, 0.7, false);

    std::cout << "\n----- Hamiltonian Cycles Stress Test (" << numVertices << " vertices) -----\n";

    // Ensure it's connected (otherwise we know there are no Hamiltonian cycles)
    if (!g.isConnected()) {
        std::cout << "Graph is not connected, ensuring connectivity...\n";
        for (size_t i = 0; i < numVertices - 1; ++i) {
            g.addEdge(i, i + 1);
            g.addEdge(i + 1, i);
        }
        g.addEdge(numVertices - 1, 0);
        g.addEdge(0, numVertices - 1);
    }

    // Count edges
    int edgeCount = 0;
    for (size_t i = 0; i < g.getNumVertices(); ++i)
        edgeCount += g.getNeighbors(i).size();

    std::cout << "Graph with " << g.getNumVertices() << " vertices and " << edgeCount << " edges\n";
    std::cout << "Graph structure:\n" << g.toString() << std::endl;

    std::vector<std::vector<int>> cycles;
    measureExecutionTime(
        "Hamiltonian Cycles Algorithm", [&]() { cycles = g.findHamiltonianCycles(); });

    std::cout << "Found " << cycles.size() << " Hamiltonian cycles\n";

    if (!cycles.empty()) {
        std::cout << "First cycle: ";
        for (int vertex : cycles[0]) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
}

// New test for Strongly Connected Components
TEST_F(GraphTest, StronglyConnectedComponents_Performance)
{
    // Create a graph with known strongly connected components
    Graph g(8, false);

    // SCC 1: 0-1-2
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    // SCC 2: 3-4
    g.addEdge(3, 4);
    g.addEdge(4, 3);

    // SCC 3: 5-6-7
    g.addEdge(5, 6);
    g.addEdge(6, 7);
    g.addEdge(7, 5);

    // Connections between SCCs
    g.addEdge(0, 3);
    g.addEdge(4, 5);
    g.addEdge(2, 5);
    g.addEdge(7, 4);

    std::cout << "\n----- Strongly Connected Components Test -----\n";
    std::cout << "Graph:\n" << g.toString() << std::endl;

    // Since your Graph class doesn't have a direct SCC method, we'll test using isStronglyConnected
    std::cout << "Testing strong connectivity between vertices:\n";

    auto testSCC = [&]() {
        // Test within the same SCC (should be strongly connected)
        bool scc1 = g.areVerticesStronglyConnected(0, 1) && g.areVerticesStronglyConnected(1, 2)
            && g.areVerticesStronglyConnected(2, 0);
        bool scc2 = g.areVerticesStronglyConnected(3, 4);
        bool scc3 = g.areVerticesStronglyConnected(5, 6) && g.areVerticesStronglyConnected(6, 7)
            && g.areVerticesStronglyConnected(7, 5);

        // Test between different SCCs (should not be strongly connected)
        bool between1and2 = !g.areVerticesStronglyConnected(0, 3);
        bool between1and3 = !g.areVerticesStronglyConnected(0, 5);

        return scc1 && scc2 && scc3 && between1and2 && between1and3;
    };

    bool result;
    measureExecutionTime("Strongly Connected Components Check", [&]() { result = testSCC(); });

    std::cout << "SCC test " << (result ? "passed" : "failed") << std::endl;
    EXPECT_TRUE(result);
}

// Stress test for Strongly Connected Components
TEST_F(GraphTest, StronglyConnectedComponents_StressTest)
{
    const size_t numVertices = 15;
    Graph g = createRandomGraph(numVertices, 0.3, false);

    std::cout << "\n----- Strongly Connected Components Stress Test (" << numVertices
              << " vertices) -----\n";

    // Count edges
    int edgeCount = 0;
    for (size_t i = 0; i < g.getNumVertices(); ++i)
        edgeCount += g.getNeighbors(i).size();

    std::cout << "Graph with " << g.getNumVertices() << " vertices and " << edgeCount << " edges\n";
    std::cout << "Graph structure:\n" << g.toString() << std::endl;

    // Test strong connectivity of the entire graph
    bool isStronglyConnected;
    measureExecutionTime(
        "isStronglyConnected Algorithm", [&]() { isStronglyConnected = g.isStronglyConnected(); });

    std::cout << "Graph is "
              << (isStronglyConnected ? "strongly connected" : "not strongly connected")
              << std::endl;

    // If not strongly connected, test random vertex pairs
    if (!isStronglyConnected) {
        std::cout << "Testing random vertex pairs for strong connectivity:\n";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> vertexDist(0, numVertices - 1);

        for (int i = 0; i < 5; ++i) {
            size_t u = vertexDist(gen);
            size_t v = vertexDist(gen);

            while (u == v) {
                v = vertexDist(gen);
            }

            bool areConnected;
            std::string testDescription
                = "SCC check between vertices " + std::to_string(u) + " and " + std::to_string(v);

            measureExecutionTime(
                testDescription, [&]() { areConnected = g.areVerticesStronglyConnected(u, v); });

            std::cout << "Vertices " << u << " and " << v << " are "
                      << (areConnected ? "strongly connected" : "not strongly connected")
                      << std::endl;
        }
    }
}

// Add a moderate graph test to analyze multiple algorithms at once
TEST_F(GraphTest, AlgorithmPerformanceComparison)
{
    std::cout << "\n===== Algorithm Performance Comparison =====\n";

    // Using smaller sizes to keep runtime manageable
    for (size_t size : { 5, 7, 9 }) {
        std::cout << "\n--- Testing with " << size << " vertices ---\n";

        // Create a connected graph suitable for all algorithms
        Graph g = createConnectedGraph(size, 0.4);

        // Ensure it's complete for TSP
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                if (i != j && !g.isAdjacent(i, j)) {
                    g.addEdge(i, j, 100); // High weight for edges we're adding to make it complete
                }
            }
        }

        // Count edges
        int edgeCount = 0;
        for (size_t i = 0; i < g.getNumVertices(); ++i) {
            edgeCount += g.getNeighbors(i).size();
        }
        edgeCount /= 2; // For undirected edges

        std::cout << "Graph with " << size << " vertices and " << edgeCount << " edges\n";
        std::cout << "Graph structure:\n" << g.toString() << std::endl;

        // Test MST
        Graph mst;
        measureExecutionTime("MST Algorithm", [&]() { mst = g.minimumSpanningTree(); });

        int mstWeight = calculateMSTWeight(mst);
        std::cout << "MST weight: " << mstWeight << std::endl;
        std::cout << "MST structure:\n" << mst.toString() << std::endl;

        // Test TSP
        std::pair<std::vector<int>, int> tspResult;
        measureExecutionTime("TSP Algorithm", [&]() { tspResult = g.travelingSalesman(); });

        std::cout << "TSP cost: " << tspResult.second << std::endl;
        std::cout << "TSP path: ";
        for (int vertex : tspResult.first) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;

        // Test Hamiltonian Cycles
        std::vector<std::vector<int>> cycles;
        measureExecutionTime(
            "Hamiltonian Cycles Algorithm", [&]() { cycles = g.findHamiltonianCycles(); });

        std::cout << "Found " << cycles.size() << " Hamiltonian cycles\n";
        if (!cycles.empty()) {
            std::cout << "First cycle: ";
            for (int vertex : cycles[0]) {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        }

        // Test Strongly Connected
        bool isStronglyConnected;
        measureExecutionTime("isStronglyConnected Algorithm",
            [&]() { isStronglyConnected = g.isStronglyConnected(); });

        std::cout << "Graph is "
                  << (isStronglyConnected ? "strongly connected" : "not strongly connected")
                  << std::endl;
    }
}
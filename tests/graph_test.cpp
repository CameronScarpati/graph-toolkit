#include "../include/Graph.h"
#include <gtest/gtest.h>

// The fixture for testing the Graph Class.
class GraphTest : public ::testing::Test { };

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

TEST_F(GraphTest, MoveConstructor)
{
    Graph g1(4);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);

    Graph g2(std::move(g1));
    EXPECT_EQ(g2.getNumVertices(), 4);
    EXPECT_FALSE(g.getIsWeighted());
    EXPECT_TRUE(g2.isAdjacent(0, 1));
    EXPECT_TRUE(g2.isAdjacent(1, 2));
    EXPECT_TRUE(g2.isAdjacent(2, 3));
    EXPECT_FALSE(g2.isAdjacent(0, 2));

    EXPECT_EQ(g1.getNumVertices(), 0);
}

TEST_F(GraphTest, CopyAssignment)
{
    Graph g1(4);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);

    Graph g2;
    g2 = g1;
    EXPECT_EQ(g2.getNumVertices(), 4);
    EXPECT_FALSE(g.getIsWeighted());
    EXPECT_TRUE(g2.isAdjacent(0, 1));
    EXPECT_TRUE(g2.isAdjacent(1, 2));
    EXPECT_TRUE(g2.isAdjacent(2, 3));
    EXPECT_FALSE(g2.isAdjacent(0, 2));
}

TEST_F(GraphTest, MoveAssignment)
{
    Graph g1(4);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);

    Graph g2;
    g2 = std::move(g1);
    EXPECT_EQ(g2.getNumVertices(), 4);
    EXPECT_FALSE(g.getIsWeighted());
    EXPECT_TRUE(g2.isAdjacent(0, 1));
    EXPECT_TRUE(g2.isAdjacent(1, 2));
    EXPECT_TRUE(g2.isAdjacent(2, 3));
    EXPECT_FALSE(g2.isAdjacent(0, 2));

    EXPECT_EQ(g1.getNumVertices(), 0);
}

TEST_F(GraphTest, AddVertex)
{
    Graph g(3);
    EXPECT_EQ(g.getNumVertices(), 3);


    g.addVertex();
    EXPECT_EQ(g.getNumVertices(), 4);

    // Test-edge connections with new vertex
    EXPECT_NO_THROW({
        g.addEdge(0, 3);
        g.addEdge(3, 1);
        EXPECT_TRUE(g.isAdjacent(0, 3));
        EXPECT_TRUE(g.isAdjacent(3, 1));
    });
}

TEST_F(GraphTest, RemoveVertex)
{
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 0);

    EXPECT_TRUE(g.isAdjacent(0, 1));
    EXPECT_TRUE(g.isAdjacent(2, 3));

    g.removeVertex(1);
    EXPECT_EQ(g.getNumVertices(), 3);

    // After removing vertex 1, vertex 2 becomes vertex 1 and vertex 3 becomes vertex 2
    EXPECT_TRUE(g.isAdjacent(0, 1)); // This was 0->2 before
    EXPECT_TRUE(g.isAdjacent(1, 2)); // This was 2->3 before

    EXPECT_THROW(g.isAdjacent(0, 3), std::out_of_range);
    EXPECT_THROW(g.removeVertex(4), std::out_of_range);
}

TEST_F(GraphTest, AddRemoveEdge)
{
    Graph g(4);

    EXPECT_FALSE(g.isAdjacent(0, 1));
    g.addEdge(0, 1);
    EXPECT_TRUE(g.isAdjacent(0, 1));

    EXPECT_FALSE(g.isAdjacent(1, 0)); // Directed graph

    g.removeEdge(0, 1);
    EXPECT_FALSE(g.isAdjacent(0, 1));

    EXPECT_THROW(g.addEdge(0, 4), std::out_of_range);
    EXPECT_THROW(g.removeEdge(4, 0), std::out_of_range);
}

TEST_F(GraphTest, GetNeighbors)
{
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 4);

    std::vector<int> neighbors = g.getNeighbors(0);
    EXPECT_EQ(neighbors.size(), 3);
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), 1) != neighbors.end());
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), 2) != neighbors.end());
    EXPECT_TRUE(std::find(neighbors.begin(), neighbors.end(), 4) != neighbors.end());

    neighbors = g.getNeighbors(3);
    EXPECT_EQ(neighbors.size(), 0);

    EXPECT_THROW(g.getNeighbors(5), std::out_of_range);
}

TEST_F(GraphTest, GetDegree)
{
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 4);

    EXPECT_EQ(g.getDegree(0), 3);
    EXPECT_EQ(g.getDegree(1), 0);

    g.addEdge(1, 0);
    g.addEdge(1, 3);
    EXPECT_EQ(g.getDegree(1), 2);

    EXPECT_THROW(g.getDegree(5), std::out_of_range);
}

TEST_F(GraphTest, IsConnected)
{
    // Isolated vertices - not connected
    Graph g1(3);
    EXPECT_FALSE(g1.isConnected());

    // Connected graph
    Graph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0);
    EXPECT_TRUE(g2.isConnected());

    // Partially connected graph
    Graph g3(4);
    g3.addEdge(0, 1);
    g3.addEdge(1, 0);
    g3.addEdge(2, 3);
    g3.addEdge(3, 2);
    EXPECT_FALSE(g3.isConnected());
}

TEST_F(GraphTest, IsStronglyConnected)
{
    // Isolated vertices - not strongly connected
    Graph g1(3);
    EXPECT_FALSE(g1.isStronglyConnected());

    // Strongly connected graph
    Graph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0);
    EXPECT_TRUE(g2.isStronglyConnected());

    // Not strongly connected
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(1, 2);
    EXPECT_FALSE(g3.isStronglyConnected());
}

TEST_F(GraphTest, HasCycle)
{
    // No cycle
    Graph g1(3);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    EXPECT_FALSE(g1.hasCycle());

    // Has cycle
    Graph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0);
    EXPECT_TRUE(g2.hasCycle());

    // Self-loop
    Graph g3(1);
    g3.addEdge(0, 0);
    EXPECT_TRUE(g3.hasCycle());
}

TEST_F(GraphTest, IsComplete)
{
    // Empty graph is complete by definition
    Graph g1(0);
    EXPECT_TRUE(g1.isComplete());

    // Single vertex is complete without a self-loop
    Graph g2(1);
    EXPECT_TRUE(g2.isComplete());

    g2.addEdge(0, 0);
    EXPECT_TRUE(g2.isComplete());

    // Complete graph
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(0, 2);
    g3.addEdge(1, 0);
    g3.addEdge(1, 2);
    g3.addEdge(2, 0);
    g3.addEdge(2, 1);
    EXPECT_TRUE(g3.isComplete());

    // Not complete
    Graph g4(3);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    EXPECT_FALSE(g4.isComplete());
}

TEST_F(GraphTest, FindHamiltonianCycles)
{
    // Single vertex with self-loop
    Graph g1(1);
    g1.addEdge(0, 0);
    auto cycles = g1.findHamiltonianCycles();
    EXPECT_EQ(cycles.size(), 1);
    EXPECT_EQ(cycles[0], std::vector<int>({ 0, 0 }));

    // Single vertex without self-loop
    Graph g2(1);
    cycles = g2.findHamiltonianCycles();
    EXPECT_EQ(cycles.size(), 0);

    // Complete graph with 3 vertices
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(0, 2);
    g3.addEdge(1, 0);
    g3.addEdge(1, 2);
    g3.addEdge(2, 0);
    g3.addEdge(2, 1);

    cycles = g3.findHamiltonianCycles();
    EXPECT_FALSE(cycles.empty());

    // Verify cycles - should have length numVertices + 1
    for (const auto& cycle : cycles) {
        EXPECT_EQ(cycle.size(), g3.getNumVertices() + 1);
        EXPECT_EQ(cycle.front(), cycle.back());
    }

    // No Hamiltonian cycle
    Graph g4(3);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    cycles = g4.findHamiltonianCycles();
    EXPECT_TRUE(cycles.empty());
}

TEST_F(GraphTest, HasHamiltonianCycle)
{
    // Single vertex with self-loop
    Graph g1(1);
    g1.addEdge(0, 0);
    EXPECT_TRUE(g1.hasHamiltonianCycle());

    // Single vertex without self-loop
    Graph g2(1);
    EXPECT_FALSE(g2.hasHamiltonianCycle());

    // Complete graph with 3 vertices
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(0, 2);
    g3.addEdge(1, 0);
    g3.addEdge(1, 2);
    g3.addEdge(2, 0);
    g3.addEdge(2, 1);
    EXPECT_TRUE(g3.hasHamiltonianCycle());

    // No Hamiltonian cycle
    Graph g4(3);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    EXPECT_FALSE(g4.hasHamiltonianCycle());
}

TEST_F(GraphTest, DepthFirstTraversal)
{
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    std::vector<int> traversal = g.depthFirstTraversal(0);
    EXPECT_EQ(traversal.size(), 5);
    EXPECT_EQ(traversal[0], 0);

    // Test for invalid vertex
    EXPECT_THROW(g.depthFirstTraversal(5), std::out_of_range);

    // Test disconnected graph
    Graph g2(5);
    g2.addEdge(0, 1);
    g2.addEdge(2, 3);
    g2.addEdge(3, 4);

    traversal = g2.depthFirstTraversal(0);
    EXPECT_EQ(traversal.size(), 2);
    EXPECT_EQ(traversal[0], 0);
    EXPECT_EQ(traversal[1], 1);

    traversal = g2.depthFirstTraversal(2);
    EXPECT_EQ(traversal.size(), 3);
    EXPECT_EQ(traversal[0], 2);
}

TEST_F(GraphTest, BreadthFirstTraversal)
{
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);

    std::vector<int> traversal = g.breadthFirstTraversal(0);
    EXPECT_EQ(traversal.size(), 5);
    EXPECT_EQ(traversal[0], 0);

    // Test for invalid vertex
    EXPECT_THROW(g.breadthFirstTraversal(5), std::out_of_range);

    // Test disconnected graph
    Graph g2(5);
    g2.addEdge(0, 1);
    g2.addEdge(2, 3);
    g2.addEdge(3, 4);

    traversal = g2.breadthFirstTraversal(0);
    EXPECT_EQ(traversal.size(), 2);
    EXPECT_EQ(traversal[0], 0);
    EXPECT_EQ(traversal[1], 1);

    traversal = g2.breadthFirstTraversal(2);
    EXPECT_EQ(traversal.size(), 3);
    EXPECT_EQ(traversal[0], 2);
}

TEST_F(GraphTest, Clear)
{
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    EXPECT_EQ(g.getNumVertices(), 5);

    g.clear();
    EXPECT_EQ(g.getNumVertices(), 0);
    EXPECT_THROW(g.isAdjacent(0, 1), std::out_of_range);
}

TEST_F(GraphTest, ToString)
{
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    std::string str = g.toString();
    EXPECT_FALSE(str.empty());

    // Basic verification - should contain all vertices
    EXPECT_NE(str.find('0'), std::string::npos);
    EXPECT_NE(str.find('1'), std::string::npos);
    EXPECT_NE(str.find('2'), std::string::npos);
}

TEST_F(GraphTest, AntiTests)
{
    // These tests verify that certain operations should NOT compile
    // They are commented out so the suite compiles

    // Graph g(3);
    // Graph g2 = 5; // Should not be allowed

    // int vertex = g; // Implicit conversion should not be allowed
}
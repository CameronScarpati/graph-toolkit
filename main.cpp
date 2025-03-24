#include "Graph.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

/**
 * Test function to run all tests and report results
 */
void runAllTests() {
  int passedTests = 0;
  int totalTests = 0;

  auto runTest = [&passedTests, &totalTests](bool condition,
                                             const std::string &testName) {
    totalTests++;
    if (condition) {
      std::cout << "✓ PASSED: " << testName << std::endl;
      passedTests++;
    } else {
      std::cout << "✗ FAILED: " << testName << std::endl;
    }
  };

  // Constructor Tests
  {
    Graph g1;
    runTest(g1.getNumVertices() == 0,
            "Default constructor creates empty graph");

    Graph g2(5);
    runTest(g2.getNumVertices() == 5,
            "Parameterized constructor creates graph with correct size");

    Graph g3(g2);
    runTest(g3.getNumVertices() == g2.getNumVertices(),
            "Copy constructor works correctly");

    Graph g4 = g2;
    runTest(g4.getNumVertices() == g2.getNumVertices(),
            "Copy assignment works correctly");

    Graph g5 = std::move(Graph(3));
    runTest(g5.getNumVertices() == 3, "Move constructor works correctly");

    Graph g6;
    g6 = std::move(Graph(4));
    runTest(g6.getNumVertices() == 4, "Move assignment works correctly");
  }

  // Basic Graph Modification Tests
  {
    Graph g(3);

    // Add/Remove Vertex Tests
    g.addVertex();
    runTest(g.getNumVertices() == 4, "addVertex increases vertex count");

    g.removeVertex(2);
    runTest(g.getNumVertices() == 3, "removeVertex decreases vertex count");

    // Add/Remove Edge Tests
    g.addEdge(0, 1);
    runTest(g.isAdjacent(0, 1), "addEdge creates edge correctly");
    runTest(!g.isAdjacent(1, 0), "addEdge creates directed edge only");

    g.removeEdge(0, 1);
    runTest(!g.isAdjacent(0, 1), "removeEdge removes edge correctly");

    // Edge error conditions
    bool exceptionCaught = false;
    try {
      g.addEdge(0, 5); // Vertex 5 doesn't exist
    } catch (...) {
      exceptionCaught = true;
    }
    runTest(exceptionCaught, "addEdge throws exception for invalid vertex");
  }

  // Neighbor and Degree Tests
  {
    Graph g(4);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);

    auto neighbors = g.getNeighbors(0);
    runTest(neighbors.size() == 2,
            "getNeighbors returns correct number of neighbors");
    runTest(
        std::find(neighbors.begin(), neighbors.end(), 1) != neighbors.end() &&
            std::find(neighbors.begin(), neighbors.end(), 2) != neighbors.end(),
        "getNeighbors returns correct neighbors");

    runTest(g.getDegree(0) == 2, "getDegree returns correct out-degree");
    runTest(g.getDegree(3) == 0,
            "getDegree returns 0 for vertex with no edges");
  }

  // Graph Property Tests
  {
    // Connected graph test
    Graph connected(3);
    connected.addEdge(0, 1);
    connected.addEdge(1, 0);
    connected.addEdge(1, 2);
    connected.addEdge(2, 1);
    runTest(connected.isConnected(),
            "isConnected returns true for connected graph");

    // Disconnected graph test
    Graph disconnected(3);
    disconnected.addEdge(0, 1);
    disconnected.addEdge(1, 0);
    runTest(!disconnected.isConnected(),
            "isConnected returns false for disconnected graph");

    // Strongly connected test
    Graph stronglyConnected(3);
    stronglyConnected.addEdge(0, 1);
    stronglyConnected.addEdge(1, 2);
    stronglyConnected.addEdge(2, 0);
    runTest(stronglyConnected.isStronglyConnected(),
            "isStronglyConnected returns true for strongly connected graph");

    // Not strongly connected test
    Graph notStronglyConnected(3);
    notStronglyConnected.addEdge(0, 1);
    notStronglyConnected.addEdge(1, 2);
    runTest(
        !notStronglyConnected.isStronglyConnected(),
        "isStronglyConnected returns false for not strongly connected graph");

    // Cycle test
    Graph cyclic(3);
    cyclic.addEdge(0, 1);
    cyclic.addEdge(1, 2);
    cyclic.addEdge(2, 0);
    runTest(cyclic.hasCycle(), "hasCycle returns true for cyclic graph");

    // Acyclic test
    Graph acyclic(3);
    acyclic.addEdge(0, 1);
    acyclic.addEdge(1, 2);
    runTest(!acyclic.hasCycle(), "hasCycle returns false for acyclic graph");

    // Complete graph test
    Graph complete(3);
    complete.addEdge(0, 1);
    complete.addEdge(0, 2);
    complete.addEdge(1, 0);
    complete.addEdge(1, 2);
    complete.addEdge(2, 0);
    complete.addEdge(2, 1);
    runTest(complete.isComplete(),
            "isComplete returns true for complete graph");

    // Not complete graph test
    Graph notComplete(3);
    notComplete.addEdge(0, 1);
    notComplete.addEdge(1, 2);
    runTest(!notComplete.isComplete(),
            "isComplete returns false for incomplete graph");
  }

  // Hamiltonian Cycle Tests
  {
    // Graph with Hamiltonian cycle
    Graph hamiltonian(4);
    hamiltonian.addEdge(0, 1);
    hamiltonian.addEdge(1, 2);
    hamiltonian.addEdge(2, 3);
    hamiltonian.addEdge(3, 0);

    runTest(
        hamiltonian.hasHamiltonianCycle(),
        "hasHamiltonianCycle returns true for graph with Hamiltonian cycle");

    auto cycles = hamiltonian.findHamiltonianCycles();
    runTest(!cycles.empty(), "findHamiltonianCycles returns non-empty list for "
                             "graph with Hamiltonian cycle");

    bool validCycle = true;
    if (!cycles.empty()) {
      auto cycle = cycles[0];
      if (cycle.size() != hamiltonian.getNumVertices() + 1) {
        validCycle = false;
      } else {
        // Check that an edge connects each pair in the cycle
        for (size_t i = 0; i < cycle.size() - 1; i++) {
          if (!hamiltonian.isAdjacent(cycle[i], cycle[i + 1])) {
            validCycle = false;
            break;
          }
        }
        // Check that all vertices are in the cycle
        std::vector<bool> visited(hamiltonian.getNumVertices(), false);
        for (size_t i = 0; i < cycle.size() - 1; i++) {
          visited[cycle[i]] = true;
        }
        for (size_t i = 0; i < hamiltonian.getNumVertices(); i++) {
          if (!visited[i]) {
            validCycle = false;
            break;
          }
        }
      }
    }
    runTest(validCycle,
            "findHamiltonianCycles returns valid Hamiltonian cycle");

    // Graph without Hamiltonian cycle
    Graph nonHamiltonian(3);
    nonHamiltonian.addEdge(0, 1);
    nonHamiltonian.addEdge(1, 0);

    runTest(!nonHamiltonian.hasHamiltonianCycle(),
            "hasHamiltonianCycle returns false for graph without Hamiltonian "
            "cycle");

    auto noCycles = nonHamiltonian.findHamiltonianCycles();
    runTest(noCycles.empty(), "findHamiltonianCycles returns empty list for "
                              "graph without Hamiltonian cycle");
  }

  // Traversal Tests
  {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    // DFS test
    auto dfsResult = g.depthFirstTraversal(0);
    bool dfsCorrect = dfsResult.size() == 5;
    if (dfsCorrect) {
      // Check that all vertices are reachable
      std::vector<bool> visited(5, false);
      for (int v : dfsResult) {
        visited[v] = true;
      }
      for (size_t i = 0; i < 5; i++) {
        if (!visited[i]) {
          dfsCorrect = false;
          break;
        }
      }
    }
    runTest(dfsCorrect, "depthFirstTraversal visits all reachable vertices");

    // BFS test
    auto bfsResult = g.breadthFirstTraversal(0);
    bool bfsCorrect = bfsResult.size() == 5;
    if (bfsCorrect) {
      // Check that all vertices are reachable
      std::vector<bool> visited(5, false);
      for (int v : bfsResult) {
        visited[v] = true;
      }
      for (size_t i = 0; i < 5; i++) {
        if (!visited[i]) {
          bfsCorrect = false;
          break;
        }
      }

      // Check BFS order is correct (level-wise)
      // For this specific graph, the BFS order should be 0, 1, 2, 3, 4
      // or 0, 2, 1, 3, 4 depending on implementation
      bool validOrder = true;
      if (bfsResult[0] != 0) {
        validOrder = false;
      }

      // Check level 1 (vertices 1 and 2)
      bool level1Has1 = bfsResult[1] == 1 || bfsResult[2] == 1;
      bool level1Has2 = bfsResult[1] == 2 || bfsResult[2] == 2;
      if (!level1Has1 || !level1Has2) {
        validOrder = false;
      }

      // Check level 2 (vertex 3)
      if (bfsResult[3] != 3) {
        validOrder = false;
      }

      // Check level 3 (vertex 4)
      if (bfsResult[4] != 4) {
        validOrder = false;
      }

      bfsCorrect = bfsCorrect && validOrder;
    }
    runTest(bfsCorrect,
            "breadthFirstTraversal visits vertices in correct order");
  }

  // Utility Function Tests
  {
    Graph g(3);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);

    std::string graphStr = g.toString();
    runTest(!graphStr.empty(), "toString returns non-empty string");

    // Clear test
    g.clear();
    runTest(g.getNumVertices() == 0, "clear removes all vertices");
  }

  // Final results
  std::cout << "\n=== Test Results ===\n";
  std::cout << "Passed: " << passedTests << "/" << totalTests << " tests"
            << std::endl;
}

int main() {
  runAllTests();
  return 0;
}
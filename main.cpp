#include "Graph.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

// Helper function to report test results.
void runTest(bool condition, const std::string &testName) {
  if (condition)
    std::cout << "✓ PASSED: " << testName << std::endl;
  else
    std::cout << "✗ FAILED: " << testName << std::endl;
}

//
// SECTION 1: Constructors, Destructor, Copy/Move
//
void testConstructors() {
  std::cout << "\n[Constructors & Copy/Move]" << std::endl;

  // Default Constructor
  Graph gDefault;
  gDefault.print();
  runTest(gDefault.getNumVertices() == 0, "Default constructor creates empty graph");

  // Parameterized Constructor
  Graph gParam(5);
  gParam.print();
  runTest(gParam.getNumVertices() == 5, "Parameterized constructor creates graph with 5 vertices");

  // Copy Constructor
  gParam.addEdge(0, 1);
  Graph gCopy(gParam);
  gCopy.print();
  runTest(gCopy.getNumVertices() == gParam.getNumVertices(), "Copy constructor duplicates vertex count");
  runTest(gCopy.isAdjacent(0, 1), "Copy constructor duplicates edges");

  // Modify original to check independence
  gParam.addVertex();
  gParam.print();
  runTest(gCopy.getNumVertices() != gParam.getNumVertices(), "Copy is independent from original");

  // Copy Assignment Operator
  Graph gAssign(3);
  gAssign = gCopy;
  gAssign.print();
  runTest(gAssign.getNumVertices() == gCopy.getNumVertices(), "Copy assignment duplicates vertex count");
  runTest(gAssign.isAdjacent(0, 1), "Copy assignment duplicates edges");

  // Move Constructor
  Graph gTemp(4);
  gTemp.addEdge(0, 3);
  gTemp.print();
  Graph gMove(std::move(gTemp));
  gMove.print();
  runTest(gMove.getNumVertices() == 4, "Move constructor transfers vertex count");
  runTest(gMove.isAdjacent(0, 3), "Move constructor transfers edges");

  // Move Assignment Operator
  Graph gTemp2(4);
  gTemp2.addEdge(1, 2);
  gTemp2.print();
  Graph gMoveAssign;
  gMoveAssign = std::move(gTemp2);
  gMoveAssign.print();
  runTest(gMoveAssign.getNumVertices() == 4, "Move assignment transfers vertex count");
  runTest(gMoveAssign.isAdjacent(1, 2), "Move assignment transfers edges");
}

//
// SECTION 2: Vertex Addition and Removal
//
void testVertexModification() {
  std::cout << "\n[Vertex Addition/Removal]" << std::endl;
  Graph g(3);
  g.print();

  // Test addVertex
  g.addVertex();
  g.print();
  runTest(g.getNumVertices() == 4, "addVertex increases vertex count");

  // Test removeVertex (valid index)
  g.removeVertex(2);
  g.print();
  runTest(g.getNumVertices() == 3, "removeVertex decreases vertex count");

  // Remove vertex that has edges
  g.addEdge(0, 1);
  g.addEdge(1, 0);
  g.print();
  g.removeVertex(1);
  g.print();
  runTest(g.getNumVertices() == 2, "removeVertex updates edges when vertex is removed");

  // Test removeVertex with invalid index
  bool exceptionCaught = false;
  try {
    g.removeVertex(10);
  } catch (const std::out_of_range &) {
    exceptionCaught = true;
  }
  runTest(exceptionCaught, "removeVertex throws exception for invalid index");
}

//
// SECTION 3: Edge Addition and Removal
//
void testEdgeModification() {
  std::cout << "\n[Edge Addition/Removal]" << std::endl;
  Graph g(4);
  g.print();

  // Test addEdge basic
  g.addEdge(0, 1);
  g.print();
  runTest(g.isAdjacent(0, 1), "addEdge creates directed edge correctly");
  runTest(!g.isAdjacent(1, 0), "addEdge creates directed edge only");

  // Test addEdge duplicate: adding same edge should not increase degree.
  g.addEdge(0, 1);
  runTest(g.getDegree(0) == 1, "Duplicate addEdge does not increase degree");

  // Test addEdge self-loop
  g.addEdge(2, 2);
  g.print();
  runTest(g.isAdjacent(2, 2), "addEdge allows self-loop");
  runTest(g.getDegree(2) == 1, "Self-loop increases degree by 1");

  // Test addEdge invalid indices
  bool exceptionCaught = false;
  try {
    g.addEdge(0, 10);
  } catch (const std::out_of_range &) {
    exceptionCaught = true;
  }
  runTest(exceptionCaught, "addEdge throws exception for invalid destination vertex");

  exceptionCaught = false;
  try {
    g.addEdge(-1, 2);
  } catch (const std::out_of_range &) {
    exceptionCaught = true;
  }
  runTest(exceptionCaught, "addEdge throws exception for negative source vertex");

  // Test removeEdge basic
  g.removeEdge(0, 1);
  g.print();
  runTest(!g.isAdjacent(0, 1), "removeEdge removes edge correctly");

  // Test removeEdge on non-existent edge
  int originalDegree = g.getDegree(0);
  g.removeEdge(0, 1);
  runTest(g.getDegree(0) == originalDegree, "removeEdge on non-existent edge leaves degree unchanged");

  // Test removeEdge with invalid indices
  exceptionCaught = false;
  try {
    g.removeEdge(10, 0);
  } catch (const std::out_of_range &) {
    exceptionCaught = true;
  }
  runTest(exceptionCaught, "removeEdge throws exception for invalid vertex indices");
}

//
// SECTION 4: Adjacency Queries and Vertex Properties
//
void testAdjacencyAndProperties() {
  std::cout << "\n[Adjacency & Vertex Properties]" << std::endl;
  Graph g(4);
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 2);
  g.print();

  // Test isAdjacent
  runTest(g.isAdjacent(0, 1), "isAdjacent returns true for existing edge");
  runTest(!g.isAdjacent(1, 0), "isAdjacent returns false when reverse edge missing");

  // Test getNeighbors
  auto neighbors = g.getNeighbors(0);
  bool correctNeighbors = (neighbors.size() == 2) &&
                          (std::find(neighbors.begin(), neighbors.end(), 1) != neighbors.end()) &&
                          (std::find(neighbors.begin(), neighbors.end(), 2) != neighbors.end());
  runTest(correctNeighbors, "getNeighbors returns correct neighbors for vertex 0");

  // Test getDegree
  runTest(g.getDegree(0) == 2, "getDegree returns correct degree for vertex 0");
  runTest(g.getDegree(3) == 0, "getDegree returns 0 for vertex with no edges");

  // Test invalid getNeighbors (should throw)
  bool exceptionCaught = false;
  try {
    g.getNeighbors(10);
  } catch (const std::out_of_range &) {
    exceptionCaught = true;
  }
  runTest(exceptionCaught, "getNeighbors throws exception for invalid vertex");
}

//
// SECTION 5: Connectivity Tests (isConnected, isStronglyConnected)
//
void testConnectivity() {
  std::cout << "\n[Connectivity]" << std::endl;
  // Weakly connected graph
  Graph gWeak(4);
  gWeak.addEdge(0, 1);
  gWeak.addEdge(1, 2);
  gWeak.addEdge(2, 3);
  gWeak.print();
  runTest(gWeak.isConnected(), "isConnected returns true for weakly connected chain");

  // Disconnected graph
  Graph gDisc(4);
  gDisc.addEdge(0, 1);
  gDisc.addEdge(2, 3);
  gDisc.print();
  runTest(!gDisc.isConnected(), "isConnected returns false for disconnected graph");

  // Single vertex graph
  Graph gSingle(1);
  gSingle.print();
  runTest(gSingle.isConnected(), "isConnected returns true for single vertex graph");

  // Strongly connected graph (cycle)
  Graph gStrong(3);
  gStrong.addEdge(0, 1);
  gStrong.addEdge(1, 2);
  gStrong.addEdge(2, 0);
  gStrong.print();
  runTest(gStrong.isStronglyConnected(), "isStronglyConnected returns true for cycle graph");

  // Not strongly connected graph
  Graph gNotStrong(3);
  gNotStrong.addEdge(0, 1);
  gNotStrong.addEdge(1, 2);
  gNotStrong.print();
  runTest(!gNotStrong.isStronglyConnected(), "isStronglyConnected returns false for one-way chain");

  // Empty graph connectivity (assumed vacuously false)
  Graph gEmpty;
  gEmpty.print();
  runTest(!gEmpty.isConnected(), "isConnected on empty graph returns false (vacuous)");
}

//
// SECTION 6: Cycle Detection (hasCycle)
//
void testCycleDetection() {
  std::cout << "\n[Cycle Detection]" << std::endl;
  // Acyclic graph (DAG)
  Graph gDAG(4);
  gDAG.addEdge(0, 1);
  gDAG.addEdge(1, 2);
  gDAG.addEdge(2, 3);
  gDAG.print();
  runTest(!gDAG.hasCycle(), "hasCycle returns false for acyclic graph");

  // Self-loop cycle
  Graph gSelf(3);
  gSelf.addEdge(1, 1);
  gSelf.print();
  runTest(gSelf.hasCycle(), "hasCycle returns true for self-loop");

  // Simple cycle
  Graph gCycle(3);
  gCycle.addEdge(0, 1);
  gCycle.addEdge(1, 2);
  gCycle.addEdge(2, 0);
  gCycle.print();
  runTest(gCycle.hasCycle(), "hasCycle returns true for simple cycle");

  // Break the cycle
  gCycle.removeEdge(2, 0);
  gCycle.print();
  runTest(!gCycle.hasCycle(), "hasCycle returns false after cycle is broken");
}

//
// SECTION 7: Complete Graph Check (isComplete)
//
void testCompleteGraph() {
  std::cout << "\n[Complete Graph Check]" << std::endl;
  // Single vertex graph
  Graph g1(1);
  g1.print();
  runTest(g1.isComplete(), "isComplete returns true for single vertex graph");

  // 2-vertex incomplete graph
  Graph g2(2);
  g2.print();
  runTest(!g2.isComplete(), "isComplete returns false for 2 vertices with no edge");
  g2.addEdge(0, 1);
  g2.print();
  runTest(!g2.isComplete(), "isComplete false when only one directed edge exists");
  g2.addEdge(1, 0);
  g2.print();
  runTest(g2.isComplete(), "isComplete returns true for 2 vertices with bidirectional edge");

  // 3-vertex fully connected directed graph
  Graph g3(3);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (i != j)
        g3.addEdge(i, j);
  g3.print();
  runTest(g3.isComplete(), "isComplete returns true for fully connected 3-vertex graph");

  // Remove one edge to break completeness
  g3.removeEdge(0, 1);
  g3.print();
  runTest(!g3.isComplete(), "isComplete returns false after removing an edge");
}

//
// SECTION 8: Hamiltonian Cycle Tests
//
void testHamiltonianCycles() {
  std::cout << "\n[Hamiltonian Cycle Tests]" << std::endl;
  // Graph without Hamiltonian cycle
  Graph gNonHam(3);
  gNonHam.addEdge(0, 1);
  gNonHam.addEdge(1, 0);
  gNonHam.print();
  runTest(!gNonHam.hasHamiltonianCycle(), "hasHamiltonianCycle returns false for graph without Hamiltonian cycle");
  runTest(gNonHam.findHamiltonianCycles().empty(), "findHamiltonianCycles returns empty for graph without Hamiltonian cycle");

  // Graph with Hamiltonian cycle (simple cycle)
  Graph gHam(4);
  gHam.addEdge(0, 1);
  gHam.addEdge(1, 2);
  gHam.addEdge(2, 3);
  gHam.addEdge(3, 0);
  gHam.print();
  runTest(gHam.hasHamiltonianCycle(), "hasHamiltonianCycle returns true for Hamiltonian cycle graph");
  auto cycles = gHam.findHamiltonianCycles();
  runTest(!cycles.empty(), "findHamiltonianCycles returns non-empty for graph with Hamiltonian cycle");

  // Validate one cycle
  bool validCycle = true;
  if (!cycles.empty()) {
    auto cycle = cycles[0];
    if (cycle.size() != gHam.getNumVertices() + 1)
      validCycle = false;
    else {
      // Check edge connectivity along the cycle.
      for (size_t i = 0; i < cycle.size() - 1; i++) {
        if (!gHam.isAdjacent(cycle[i], cycle[i + 1])) {
          validCycle = false;
          break;
        }
      }
      // Check that each vertex is visited exactly once (except start/end).
      std::vector<bool> visited(gHam.getNumVertices(), false);
      for (size_t i = 0; i < cycle.size() - 1; i++) {
        visited[cycle[i]] = true;
      }
      for (bool flag : visited)
        if (!flag) {
          validCycle = false;
          break;
        }
    }
  }
  runTest(validCycle, "findHamiltonianCycles returns a valid Hamiltonian cycle");

  // Fully connected graph (4 vertices) should yield multiple cycles.
  Graph gFull(4);
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      if (i != j)
        gFull.addEdge(i, j);
  gFull.print();
  runTest(gFull.hasHamiltonianCycle(), "hasHamiltonianCycle returns true for fully connected graph");
  auto fullCycles = gFull.findHamiltonianCycles();
  runTest(fullCycles.size() > 1, "findHamiltonianCycles returns multiple cycles for complete graph");
}

//
// SECTION 9: Graph Traversals (DFS and BFS)
//
void testTraversals() {
  std::cout << "\n[Graph Traversals]" << std::endl;
  Graph g(6);
  // Build a sample graph:
  // 0 -> 1, 0 -> 2
  // 1 -> 3, 2 -> 3
  // 3 -> 4, 3 -> 5
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 3);
  g.addEdge(2, 3);
  g.addEdge(3, 4);
  g.addEdge(3, 5);
  g.print();

  // DFS Test
  auto dfsResult = g.depthFirstTraversal(0);
  bool dfsVisitedAll = (dfsResult.size() == 6);
  if (dfsVisitedAll) {
    std::vector<bool> visited(6, false);
    for (int v : dfsResult) {
      if (v < 0 || v >= 6) { dfsVisitedAll = false; break; }
      visited[v] = true;
    }
    for (bool flag : visited)
      if (!flag) { dfsVisitedAll = false; break; }
  }
  runTest(dfsVisitedAll, "depthFirstTraversal visits all reachable vertices without duplication");

  // BFS Test
  auto bfsResult = g.breadthFirstTraversal(0);
  bool bfsVisitedAll = (bfsResult.size() == 6);
  if (bfsVisitedAll) {
    std::vector<bool> visited(6, false);
    for (int v : bfsResult) {
      if (v < 0 || v >= 6) { bfsVisitedAll = false; break; }
      visited[v] = true;
    }
    for (bool flag : visited)
      if (!flag) { bfsVisitedAll = false; break; }
  }
  runTest(bfsVisitedAll, "breadthFirstTraversal visits all reachable vertices without duplication");

  // Check expected BFS order for our graph (order may vary but first element must be 0)
  bool bfsOrderCorrect = (!bfsResult.empty() && bfsResult[0] == 0);
  runTest(bfsOrderCorrect, "breadthFirstTraversal returns expected starting element");
}

//
// SECTION 10: Clear Method
//
void testClearAndReuse() {
  std::cout << "\n[Clear Method]" << std::endl;
  Graph g(4);
  g.addEdge(0, 1);
  g.addEdge(1, 2);
  g.print();
  g.clear();
  g.print();
  runTest(g.getNumVertices() == 0, "clear() empties the graph");

  // Reuse graph after clear
  g.addVertex();
  g.addEdge(0, 0); // self-loop on the only vertex
  g.print();
  runTest(g.getNumVertices() == 1, "Graph is reusable after clear()");
  runTest(g.isAdjacent(0, 0), "Graph maintains functionality after reuse");
}

//
// SECTION 11: String Representation (toString) and Print
//
void testStringOutput() {
  std::cout << "\n[String Output]" << std::endl;
  Graph g(3);
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.print();
  std::string outStr = g.toString();
  runTest(!outStr.empty(), "toString returns non-empty string for non-empty graph");

  // Check that output contains vertex labels (simple check)
  bool containsVertex0 = (outStr.find("0") != std::string::npos);
  runTest(containsVertex0, "toString output contains vertex labels");
}

//
// SECTION 12: Exception Handling & Boundary Cases
//
void testExceptionsAndBoundaries() {
  std::cout << "\n[Exceptions & Boundary Cases]" << std::endl;
  // Test operations on an empty graph
  Graph empty;
  empty.print();
  runTest(empty.getNumVertices() == 0, "Empty graph has 0 vertices");
  runTest(!empty.hasCycle(), "Empty graph has no cycle");

  // Test traversal on empty graph should throw (invalid start index)
  bool exceptionCaught = false;
  try {
    empty.depthFirstTraversal(0);
  } catch (const std::out_of_range &) {
    exceptionCaught = true;
  }
  runTest(exceptionCaught, "depthFirstTraversal throws exception on empty graph with invalid start index");

  // Test single vertex behavior
  Graph single(1);
  single.print();
  runTest(single.isConnected(), "Single vertex graph is connected");
  runTest(single.isStronglyConnected(), "Single vertex graph is strongly connected");
  runTest(single.getDegree(0) == 0, "Single vertex graph has degree 0 without self-loop");
  single.addEdge(0, 0); // add self-loop
  single.print();
  runTest(single.getDegree(0) == 1, "Self-loop on single vertex increases degree");
}

//
// Main: Run all tests
//
int main() {
  testConstructors();
  testVertexModification();
  testEdgeModification();
  testAdjacencyAndProperties();
  testConnectivity();
  testCycleDetection();
  testCompleteGraph();
  testHamiltonianCycles();
  testTraversals();
  testClearAndReuse();
  testStringOutput();
  testExceptionsAndBoundaries();

  std::cout << "\n=== All Tests Completed ===\n";
  return 0;
}

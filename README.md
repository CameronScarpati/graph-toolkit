<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++20" />
  <img src="https://img.shields.io/badge/CMake-3.27+-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake" />
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License" />
  <img src="https://img.shields.io/badge/Tests-34%20Passing-brightgreen?style=for-the-badge" alt="Tests" />
</p>

<h1 align="center">Graph Toolkit</h1>

<p align="center">
  A high-performance C++ graph library featuring classical algorithms, NP-hard solvers, and comprehensive graph analysis tools.
</p>

<p align="center">
  <a href="#features">Features</a> &nbsp;&bull;&nbsp;
  <a href="#quick-start">Quick Start</a> &nbsp;&bull;&nbsp;
  <a href="#api-overview">API Overview</a> &nbsp;&bull;&nbsp;
  <a href="#algorithms">Algorithms</a> &nbsp;&bull;&nbsp;
  <a href="#architecture">Architecture</a> &nbsp;&bull;&nbsp;
  <a href="#testing">Testing</a>
</p>

---

## Features

| Category | Capabilities |
|----------|-------------|
| **Graph Representation** | Adjacency matrix with dynamic vertex/edge management |
| **Traversals** | Iterative DFS (stack-based), BFS (queue-based) |
| **Shortest Paths** | Dijkstra's algorithm (O(E log V)), Bellman-Ford (negative weights) |
| **Spanning Trees** | Prim's MST with binary heap optimization (O(E log V)) |
| **NP-Hard Solvers** | Hamiltonian cycle enumeration, Traveling Salesman (exact) |
| **Graph Analysis** | Connectivity, strong connectivity, cycle detection, completeness |
| **Ordering** | Topological sort via Kahn's algorithm |
| **Modern C++** | C++20, full Rule of Five, move semantics, `noexcept` guarantees |

## Quick Start

### Prerequisites

- C++ compiler with **C++20** support (GCC 12+ or Clang 14+)
- **CMake** 3.27+

### Build

```bash
git clone https://github.com/CameronScarpati/graph-toolkit.git
cd graph-toolkit
cmake -B build -S .
cmake --build build
```

### Run Tests

```bash
cd build && ctest --output-on-failure
```

## API Overview

```cpp
#include "Graph.h"
#include "Algorithms.h"

// Create a weighted directed graph with 5 vertices
Graph g(5, true);

// Add edges
g.addEdge(0, 1, 10);
g.addEdge(0, 3, 5);
g.addEdge(1, 2, 1);
g.addEdge(3, 1, 3);
g.addEdge(3, 4, 2);

// Shortest paths from vertex 0
auto [distances, predecessors] = dijkstra(g, 0);
// distances = {0, 8, 9, 5, 7}

// Undirected weighted graph for MST
Graph ug(4, true);
ug.addUndirectedEdge(0, 1, 1);
ug.addUndirectedEdge(1, 2, 2);
ug.addUndirectedEdge(2, 3, 3);
ug.addUndirectedEdge(0, 3, 4);

Graph mst = ug.minimumSpanningTree();
// MST weight = 6 (edges: 0-1, 1-2, 2-3)

// Traversals
std::vector<int> dfs = g.depthFirstTraversal(0);
std::vector<int> bfs = g.breadthFirstTraversal(0);

// Graph properties
bool connected    = g.isConnected();
bool hasCycles    = g.hasCycle();
bool complete     = g.isComplete();
```

For the complete API reference, see [`docs/API.md`](docs/API.md).

## Algorithms

### Shortest Path

| Algorithm | Time Complexity | Negative Weights | Negative Cycle Detection |
|-----------|:-:|:-:|:-:|
| **Dijkstra** | O(E log V) | No | No |
| **Bellman-Ford** | O(V * E) | Yes | Yes |

### Minimum Spanning Tree

**Prim's Algorithm** with binary heap priority queue for O(E log V) performance. Produces an optimal MST for connected, undirected, weighted graphs.

### NP-Hard Problems

| Problem | Approach | Use Case |
|---------|----------|----------|
| **Hamiltonian Cycle** | Backtracking with pruning | Enumerates all cycles |
| **Traveling Salesman** | Exact brute-force permutation | Finds optimal tour |

### Graph Analysis

| Operation | Algorithm | Complexity |
|-----------|-----------|:----------:|
| Cycle Detection | Kahn's (topological sort) | O(V + E) |
| Connectivity | DFS reachability | O(V * (V + E)) |
| Strong Connectivity | Pairwise DFS | O(V^2 * (V + E)) |
| Topological Sort | Kahn's algorithm | O(V + E) |

## Architecture

```
graph-toolkit/
├── include/
│   ├── Graph.h              # Core graph class (adjacency matrix)
│   └── Algorithms.h         # Dijkstra, Bellman-Ford, topological sort
├── src/
│   ├── Graph.cpp            # Graph implementation (~540 lines)
│   └── Algorithms.cpp       # Algorithm implementations
├── tests/
│   ├── graph_test.cpp       # Core + stress tests
│   ├── algorithms_test.cpp  # Shortest path + topological sort tests
│   └── mst_benchmark_test.cpp  # MST benchmarks (50-100 vertices)
├── docs/
│   └── API.md               # Complete API reference
├── .github/workflows/
│   └── build.yml            # CI: GCC + Clang matrix, coverage, linting
└── CMakeLists.txt           # Build configuration with FetchContent (GTest)
```

### Design Decisions

- **Adjacency matrix** representation for O(1) edge lookups and simple weight storage
- **Separate algorithms module** for shortest-path and ordering algorithms, keeping the `Graph` class focused on structure and properties
- **FetchContent** for Google Test dependency -- no manual installation required
- **Generator expressions** in CMake for clean build/install separation

## Testing

**34 tests** across three test suites with full coverage of correctness and performance:

| Suite | Tests | Coverage |
|-------|:-----:|----------|
| `GraphTest` | 17 | Constructors, traversals, properties, MST, TSP, Hamiltonian cycles, edge cases, stress tests |
| `AlgorithmsTest` | 14 | Dijkstra, Bellman-Ford, topological sort, error handling |
| `MSTBenchmarkTest` | 3 | Performance benchmarks at 50 and 100 vertices (sparse + dense) |

### CI/CD Pipeline

The GitHub Actions pipeline runs on every push:

- **Compiler matrix**: GCC 13 and Clang 17
- **Code coverage**: `lcov` report generation with artifact upload
- **Linting**: `clang-format-17` style enforcement (WebKit base style)
- **All warnings as errors**: `-Wall -Werror -Wextra -pedantic`

## Contributing

See [`CONTRIBUTING.md`](CONTRIBUTING.md) for development setup, code style guidelines, and PR process.

## License

Distributed under the MIT License. See [`LICENSE`](LICENSE) for details.

---

<p align="center">
  <sub>Built by <a href="https://linkedin.com/in/cameron-scarpati">Cameron Scarpati</a></sub>
</p>

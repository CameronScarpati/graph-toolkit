# API Reference

## Class: `Graph`

An adjacency-matrix-based graph supporting directed/undirected edges, optional weights, traversals, and advanced algorithms.

Header: `#include "Graph.h"`

---

## Constructors & Assignment

| Signature | Description |
|---|---|
| `Graph()` | Default constructor. Creates an empty graph with 0 vertices. |
| `Graph(size_t vertices)` | Creates an unweighted graph with the given number of vertices. |
| `Graph(size_t vertices, bool weighted)` | Creates a graph with the given number of vertices; `weighted` enables edge weights. |
| `Graph(const Graph& other)` | Copy constructor. |
| `Graph& operator=(const Graph& other)` | Copy assignment operator. |
| `Graph(Graph&& other) noexcept` | Move constructor. |
| `Graph& operator=(Graph&& other) noexcept` | Move assignment operator. |
| `~Graph()` | Destructor. |

---

## Vertex Operations

### `void addVertex()`

Adds a new vertex to the graph. The new vertex has index `getNumVertices()` (before the call).

### `void removeVertex(size_t vertex)`

Removes the vertex at the given index. All vertices with higher indices are shifted down by one.

- **Throws**: `std::out_of_range` if `vertex` is out of bounds.

---

## Edge Operations

### `void addEdge(size_t from, size_t to)`

Adds a directed edge from `from` to `to` with a default weight of 1.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

### `void addEdge(size_t from, size_t to, int weight)`

Adds a directed edge from `from` to `to` with the specified weight.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

### `void addUndirectedEdge(size_t from, size_t to, int weight)`

Adds edges in both directions between `from` and `to` with the specified weight.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

### `void removeEdge(size_t from, size_t to)`

Removes the directed edge from `from` to `to`.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

---

## Query Operations

### `bool isAdjacent(size_t v1, size_t v2) const`

Returns `true` if there is a directed edge from `v1` to `v2`.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

### `size_t getNumVertices() const`

Returns the number of vertices in the graph.

### `bool getIsWeighted() const`

Returns `true` if the graph was created with weighted edge support.

### `int getEdgeWeight(size_t from, size_t to) const`

Returns the weight of the edge from `from` to `to`. Returns 0 if no edge exists.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

### `std::vector<int> getNeighbors(size_t vertex) const`

Returns a vector of vertex indices adjacent to `vertex` (outgoing edges).

- **Throws**: `std::out_of_range` if `vertex` is out of bounds.

### `size_t getDegree(size_t vertex) const`

Returns the out-degree (number of outgoing edges) of the given vertex.

- **Throws**: `std::out_of_range` if `vertex` is out of bounds.

---

## Graph Properties

### `bool isConnected() const`

Returns `true` if every vertex is reachable from vertex 0 (treating edges as undirected). Returns `true` for empty graphs.

### `bool isStronglyConnected() const`

Returns `true` if every vertex is reachable from every other vertex following edge directions.

### `bool areVerticesStronglyConnected(size_t u, size_t v) const`

Returns `true` if there is a directed path from `u` to `v` **and** from `v` to `u`.

- **Throws**: `std::out_of_range` if either vertex is out of bounds.

### `bool hasCycle() const`

Returns `true` if the directed graph contains a cycle. Uses topological sort (Kahn's algorithm).

### `bool isComplete() const`

Returns `true` if every pair of distinct vertices has an edge in both directions.

---

## Traversals

### `std::vector<int> depthFirstTraversal(size_t startVertex) const`

Returns vertices in DFS order starting from `startVertex`. Uses an iterative stack-based approach. Visits all components.

- **Throws**: `std::out_of_range` if `startVertex` is out of bounds.

### `std::vector<int> breadthFirstTraversal(size_t startVertex) const`

Returns vertices in BFS order starting from `startVertex`. Uses a queue-based approach. Visits all components.

- **Throws**: `std::out_of_range` if `startVertex` is out of bounds.

---

## Advanced Algorithms

### `std::vector<std::vector<int>> findHamiltonianCycles() const`

Returns all Hamiltonian cycles in the graph. Each cycle is a vector of vertex indices starting and ending at the same vertex. Returns an empty vector if no Hamiltonian cycle exists.

- **Complexity**: O(n!) worst case (backtracking).

### `bool hasHamiltonianCycle() const`

Returns `true` if at least one Hamiltonian cycle exists.

### `Graph minimumSpanningTree() const`

Returns a new `Graph` representing the Minimum Spanning Tree using Prim's algorithm.

- **Precondition**: The graph must be connected and undirected.
- **Throws**: `std::runtime_error` if the graph is not connected.

### `std::pair<std::vector<int>, int> travelingSalesman() const`

Solves the Traveling Salesman Problem using brute-force permutation. Returns a pair of the optimal vertex ordering and the total path cost.

- **Precondition**: The graph must be connected and undirected.
- **Complexity**: O(n!) (exact solution via permutation).

---

## Utility

### `void clear()`

Removes all vertices and edges, resetting the graph to an empty state.

### `std::string toString() const`

Returns a string representation of the adjacency matrix.

### `void print() const`

Prints the adjacency matrix to `stdout`.

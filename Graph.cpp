#include "Graph.h"

bool Graph::validVertex(int vertex) const noexcept {
  return vertex >= 0 && vertex < numVertices;
}

std::vector<int>
Graph::depthFirstTraversalHelper(int startVertex,
                                 std::vector<bool> &visited) const {
  std::vector<int> result;
  std::stack<int> toTraverse;
  toTraverse.push(startVertex);
  visited[startVertex] = true;

  while (!toTraverse.empty()) {
    // Searches the current vertex (top of the stack).
    int currentVertex = toTraverse.top();
    result.push_back(currentVertex);
    toTraverse.pop();

    std::vector<int> neighbors = getNeighbors(currentVertex);

    // Marks each neighbor as visited
    for (size_t i = neighbors.size(); i-- > 0;) {
      int neighbor = neighbors[i];
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        toTraverse.push(neighbor);
      }
    }
  }
  return result;
}

void Graph::findHamiltonianCyclesHelper(
    int startVertex, int currentVertex, std::vector<int> &path,
    std::vector<bool> &visited, std::vector<std::vector<int>> &cycles) const {
  if (path.size() == numVertices) {
    if (isAdjacent(currentVertex, startVertex)) {
      std::vector<int> cycle = path;
      cycle.push_back(startVertex);
      cycles.push_back(std::move(cycle));
    }
    return;
  }

  std::vector<int> neighbors = getNeighbors(currentVertex);

  for (int neighbor : neighbors) {
    if (!visited[neighbor]) {
      visited[neighbor] = true;
      path.push_back(neighbor);

      findHamiltonianCyclesHelper(startVertex, neighbor, path, visited, cycles);

      path.pop_back();
      visited[neighbor] = false;
    }
  }
}

std::vector<int>
Graph::breadthFirstTraversalHelper(std::vector<bool> &visited,
                                   std::queue<int> &toTraverse) const {
  std::vector<int> traversal;

  while (!toTraverse.empty()) {
    int currentVertex = toTraverse.front();
    toTraverse.pop();

    traversal.push_back(currentVertex);

    std::vector<int> neighbors = getNeighbors(currentVertex);

    for (int neighbor : neighbors) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        toTraverse.push(neighbor);
      }
    }
  }
  return traversal;
}

Graph::Graph() : Graph(0) {}

Graph::Graph(int vertices) : numVertices(vertices) {
  adjacencyMatrix.resize(vertices, std::vector<int>(vertices, 0));
}

Graph::Graph(const Graph &other) {
  numVertices = other.numVertices;
  adjacencyMatrix = other.adjacencyMatrix;
}

Graph &Graph::operator=(const Graph &other) {
  if (this != &other) {
    numVertices = other.numVertices;
    adjacencyMatrix = other.adjacencyMatrix;
  }
  return *this;
}

Graph::Graph(Graph &&other) noexcept {
  numVertices = other.numVertices;
  adjacencyMatrix = std::move(other.adjacencyMatrix);

  other.numVertices = 0;
  other.clear();
}

Graph &Graph::operator=(Graph &&other) noexcept {
  if (this != &other) {
    numVertices = other.numVertices;
    adjacencyMatrix = std::move(other.adjacencyMatrix);

    other.numVertices = 0;
    other.clear();
  }
  return *this;
}

Graph::~Graph() { clear(); }

void Graph::addVertex() {
  numVertices++;

  adjacencyMatrix.emplace_back(numVertices, 0);

  for (int i = 0; i < numVertices - 1; ++i)
    adjacencyMatrix[i].push_back(0);
}

void Graph::removeVertex(int vertex) {
  if (!validVertex(vertex))
    throw std::out_of_range("This index is out of range.");

  adjacencyMatrix.erase(adjacencyMatrix.begin() + vertex);

  for (auto &row : adjacencyMatrix)
    row.erase(row.begin() + vertex);

  numVertices--;
}

void Graph::addEdge(int from, int to) {
  if (!validVertex(from) || !validVertex(to))
    throw std::out_of_range("One of these indices is out of range.");

  adjacencyMatrix[from][to] = 1;
}

void Graph::removeEdge(int from, int to) {
  if (!validVertex(from) || !validVertex(to))
    throw std::out_of_range("One of these indices is out of range.");

  adjacencyMatrix[from][to] = 0;
}

bool Graph::isAdjacent(int v1, int v2) const {
  if (!validVertex(v1) || !validVertex(v2))
    throw std::out_of_range("One of these indices is out of range.");

  return adjacencyMatrix[v1][v2] == 1;
}

size_t Graph::getNumVertices() const { return numVertices; }

std::vector<int> Graph::getNeighbors(int vertex) const {
  if (!validVertex(vertex))
    throw std::out_of_range("This index is out of range.");

  std::vector<int> neighbors;
  for (int i = 0; i < numVertices; ++i)
    if (adjacencyMatrix[vertex][i] == 1)
      neighbors.push_back(i);

  return neighbors;
}

int Graph::getDegree(int vertex) const {
  if (!validVertex(vertex))
    throw std::out_of_range("This index is out of range.");

  int count = 0;
  std::for_each(adjacencyMatrix[vertex].begin(), adjacencyMatrix[vertex].end(),
                [&count](int e) {
                  if (e)
                    ++count;
                });

  return count;
}

bool Graph::isConnected() const {
  for (int i = 0; i < numVertices; i++) {
    std::vector<int> traversal = depthFirstTraversal(i);

    if (traversal.size() == numVertices)
      return true;
  }
  return false;
}

bool Graph::isStronglyConnected() const {
  for (int i = 0; i < numVertices; i++) {
    std::vector<int> traversal = depthFirstTraversal(i);

    if (traversal.size() < numVertices)
      return false;
  }
  return true;
}

bool Graph::hasCycle() const {
  std::vector<int> inDegree(getNumVertices(), 0);
  std::queue<int> toTraverse;

  int visited = 0;

  // Calculate in-degree of each vertex.
  for (int vertex = 0; vertex < getNumVertices(); ++vertex)
    for (int edge = 0; edge < getNumVertices(); ++edge)
      if (adjacencyMatrix[vertex][edge] == 1)
        ++inDegree[edge];

  // Add all zero in-degree vertices to a queue.
  for (int u = 0; u < getNumVertices(); u++)
    if (inDegree[u] == 0)
      toTraverse.push(u);

  // Run a BFS on all zero in-degree vertices.
  while (!toTraverse.empty()) {
    int sourceVertex = toTraverse.front();
    toTraverse.pop();
    visited++;

    for (int edge = 0; edge < numVertices; ++edge) {
      if (adjacencyMatrix[sourceVertex][edge] == 1) {
        inDegree[edge]--;

        if (inDegree[edge] == 0)
          toTraverse.push(edge);
      }
    }
  }

  return visited != getNumVertices();
}

bool Graph::isComplete() const {
  for (size_t row = 0; row < numVertices; ++row)
    for (size_t col = 0; col < numVertices; ++col)
      if (row != col && adjacencyMatrix[row][col] == 0)
        return false;

  return true;
}

std::vector<std::vector<int>> Graph::findHamiltonianCycles() const {
  if (numVertices == 1 && isAdjacent(0, 0))
    return {{0, 0}};
  else if (numVertices == 1)
    return {};
  if (!isConnected())
    return {};
  if (!hasCycle())
    return {};

  std::vector<std::vector<int>> hamiltonianCycles;

  for (int startVertex = 0; startVertex < numVertices; ++startVertex) {
    std::vector<bool> visited(numVertices, false);
    std::vector<int> path;

    visited[startVertex] = true;
    path.push_back(startVertex);

    std::vector<int> neighbors = getNeighbors(startVertex);
    for (int neighbor : neighbors) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        path.push_back(neighbor);

        findHamiltonianCyclesHelper(startVertex, neighbor, path, visited, hamiltonianCycles);

        path.pop_back();
        visited[neighbor] = false;
      }
    }
  }
  return hamiltonianCycles;
}

bool Graph::hasHamiltonianCycle() const {
  return !findHamiltonianCycles().empty();
}

std::vector<int> Graph::depthFirstTraversal(int startVertex) const {
  if (!validVertex(startVertex))
    throw std::out_of_range("This index is out of range.");

  std::vector<bool> visited(numVertices, false);
  return depthFirstTraversalHelper(startVertex, visited);
}

std::vector<int> Graph::breadthFirstTraversal(int startVertex) const {
  if (!validVertex(startVertex))
    throw std::out_of_range("This index is out of range.");

  std::vector<bool> visited(numVertices, false);
  std::queue<int> toTraverse;
  toTraverse.push(startVertex);
  visited[startVertex] = true;

  std::vector<int> traversal = breadthFirstTraversalHelper(visited, toTraverse);
  return traversal;
}

void Graph::clear() {
  numVertices = 0;
  adjacencyMatrix.clear();
}

std::string Graph::toString() const {
  std::stringstream ss;

  ss << "    ";
  for (size_t i = 0; i < numVertices; ++i)
    ss << i << " ";
  ss << "\n";

  ss << "   ";
  for (size_t i = 0; i < numVertices; ++i)
    ss << "--";
  ss << "\n";

  for (size_t row = 0; row < numVertices; ++row) {
    ss << row << " | ";
    for (int col : adjacencyMatrix[row])
      ss << col << " ";
    ss << "\n";
  }

  return ss.str();
}

void Graph::print() const { std::cout << toString(); }
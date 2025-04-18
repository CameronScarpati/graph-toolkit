#include "../include/Graph.h"

bool Graph::validVertex(size_t vertex) const noexcept
{
    return vertex < numVertices;
}

std::vector<size_t> Graph::depthFirstTraversalHelper(
    size_t startVertex, std::vector<bool>& visited) const
{
    std::vector<size_t> result;
    std::stack<size_t> toTraverse;
    toTraverse.push(startVertex);
    visited[startVertex] = true;

    while (!toTraverse.empty()) {
        // Searches the current vertex (top of the stack).
        size_t currentVertex = toTraverse.top();
        result.push_back(currentVertex);
        toTraverse.pop();

        std::vector<size_t> neighbors = getNeighbors(currentVertex);

        // Marks each neighbor as visited
        for (size_t i = neighbors.size(); i-- > 0;) {
            size_t neighbor = neighbors[i];
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                toTraverse.push(neighbor);
            }
        }
    }
    return result;
}

void Graph::findHamiltonianCyclesHelper(size_t startVertex, size_t currentVertex,
    std::vector<size_t>& path, std::vector<bool>& visited,
    std::vector<std::vector<size_t>>& cycles) const
{
    if (path.size() == numVertices) {
        if (isAdjacent(currentVertex, startVertex)) {
            std::vector<size_t> cycle = path;
            cycle.push_back(startVertex);
            cycles.push_back(std::move(cycle));
        }
        return;
    }

    std::vector<size_t> neighbors = getNeighbors(currentVertex);

    for (size_t neighbor : neighbors) {
        if (!visited[neighbor]) {
            visited[neighbor] = true;
            path.push_back(neighbor);

            findHamiltonianCyclesHelper(startVertex, neighbor, path, visited, cycles);

            path.pop_back();
            visited[neighbor] = false;
        }
    }
}

std::vector<size_t> Graph::breadthFirstTraversalHelper(
    std::vector<bool>& visited, std::queue<size_t>& toTraverse) const
{
    std::vector<size_t> traversal;

    while (!toTraverse.empty()) {
        size_t currentVertex = toTraverse.front();
        toTraverse.pop();

        traversal.push_back(currentVertex);

        std::vector<size_t> neighbors = getNeighbors(currentVertex);

        for (size_t neighbor : neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                toTraverse.push(neighbor);
            }
        }
    }
    return traversal;
}

Graph::Graph()
    : Graph(0)
{
}

Graph::Graph(size_t vertices)
    : numVertices(vertices)
{
    adjacencyMatrix.resize(vertices, std::vector<size_t>(vertices, 0));
}

Graph::Graph(const Graph& other)
{
    numVertices = other.numVertices;
    adjacencyMatrix = other.adjacencyMatrix;
}

Graph& Graph::operator=(const Graph& other)
{
    if (this != &other) {
        numVertices = other.numVertices;
        adjacencyMatrix = other.adjacencyMatrix;
    }
    return *this;
}

Graph::Graph(Graph&& other) noexcept
{
    numVertices = other.numVertices;
    adjacencyMatrix = std::move(other.adjacencyMatrix);

    other.numVertices = 0;
    other.clear();
}

Graph& Graph::operator=(Graph&& other) noexcept
{
    if (this != &other) {
        numVertices = other.numVertices;
        adjacencyMatrix = std::move(other.adjacencyMatrix);

        other.numVertices = 0;
        other.clear();
    }
    return *this;
}

Graph::~Graph()
{
    clear();
}

void Graph::addVertex()
{
    numVertices++;

    adjacencyMatrix.emplace_back(numVertices, 0);

    for (size_t i = 0; i < numVertices - 1; ++i)
        adjacencyMatrix[i].push_back(0);
}

void Graph::removeVertex(size_t vertex)
{
    if (!validVertex(vertex))
        throw std::out_of_range("This index is out of range.");

    adjacencyMatrix.erase(adjacencyMatrix.begin() + vertex);

    for (auto& row : adjacencyMatrix)
        row.erase(row.begin() + vertex);

    numVertices--;
}

void Graph::addEdge(size_t from, size_t to)
{
    if (!validVertex(from) || !validVertex(to))
        throw std::out_of_range("One of these indices is out of range.");

    adjacencyMatrix[from][to] = 1;
}

void Graph::removeEdge(size_t from, size_t to)
{
    if (!validVertex(from) || !validVertex(to))
        throw std::out_of_range("One of these indices is out of range.");

    adjacencyMatrix[from][to] = 0;
}

bool Graph::isAdjacent(size_t v1, size_t v2) const
{
    if (!validVertex(v1) || !validVertex(v2))
        throw std::out_of_range("One of these indices is out of range.");

    return adjacencyMatrix[v1][v2] == 1;
}

size_t Graph::getNumVertices() const
{
    return numVertices;
}

std::vector<size_t> Graph::getNeighbors(size_t vertex) const
{
    if (!validVertex(vertex))
        throw std::out_of_range("This index is out of range.");

    std::vector<size_t> neighbors;
    for (size_t i = 0; i < numVertices; ++i)
        if (adjacencyMatrix[vertex][i] == 1)
            neighbors.push_back(i);

    return neighbors;
}

size_t Graph::getDegree(size_t vertex) const
{
    if (!validVertex(vertex))
        throw std::out_of_range("This index is out of range.");

    size_t count = 0;
    for (size_t e : adjacencyMatrix[vertex])
        if (e)
            ++count;

    return count;
}

bool Graph::isConnected() const
{
    for (size_t i = 0; i < numVertices; i++) {
        std::vector<size_t> traversal = depthFirstTraversal(i);

        if (traversal.size() == numVertices)
            return true;
    }
    return false;
}

bool Graph::isStronglyConnected() const
{
    for (size_t i = 0; i < numVertices; i++) {
        std::vector<size_t> traversal = depthFirstTraversal(i);

        if (traversal.size() < numVertices)
            return false;
    }
    return true;
}

bool Graph::hasCycle() const
{
    std::vector<size_t> inDegree(getNumVertices(), 0);
    std::queue<size_t> toTraverse;

    size_t visited = 0;

    // Calculate in-degree of each vertex.
    for (size_t vertex = 0; vertex < getNumVertices(); ++vertex)
        for (size_t edge = 0; edge < getNumVertices(); ++edge)
            if (adjacencyMatrix[vertex][edge] == 1)
                ++inDegree[edge];

    // Add all zero in-degree vertices to a queue.
    for (size_t u = 0; u < getNumVertices(); u++)
        if (inDegree[u] == 0)
            toTraverse.push(u);

    // Run a BFS on all zero in-degree vertices.
    while (!toTraverse.empty()) {
        size_t sourceVertex = toTraverse.front();
        toTraverse.pop();
        visited++;

        for (size_t edge = 0; edge < numVertices; ++edge) {
            if (adjacencyMatrix[sourceVertex][edge] == 1) {
                inDegree[edge]--;

                if (inDegree[edge] == 0)
                    toTraverse.push(edge);
            }
        }
    }

    return visited != getNumVertices();
}

bool Graph::isComplete() const
{
    for (size_t row = 0; row < numVertices; ++row)
        for (size_t col = 0; col < numVertices; ++col)
            if (row != col && adjacencyMatrix[row][col] == 0)
                return false;

    return true;
}

std::vector<std::vector<size_t>> Graph::findHamiltonianCycles() const
{
    if (numVertices == 1 && isAdjacent(0, 0))
        return { { 0, 0 } };
    else if (numVertices == 1)
        return {};
    if (!isConnected())
        return {};
    if (!hasCycle())
        return {};

    std::vector<std::vector<size_t>> hamiltonianCycles;

    for (size_t startVertex = 0; startVertex < numVertices; ++startVertex) {
        std::vector<bool> visited(numVertices, false);
        std::vector<size_t> path;

        visited[startVertex] = true;
        path.push_back(startVertex);

        std::vector<size_t> neighbors = getNeighbors(startVertex);
        for (size_t neighbor : neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                path.push_back(neighbor);

                findHamiltonianCyclesHelper(
                    startVertex, neighbor, path, visited, hamiltonianCycles);

                path.pop_back();
                visited[neighbor] = false;
            }
        }
    }
    return hamiltonianCycles;
}

bool Graph::hasHamiltonianCycle() const
{
    return !findHamiltonianCycles().empty();
}

std::vector<size_t> Graph::depthFirstTraversal(size_t startVertex) const
{
    if (!validVertex(startVertex))
        throw std::out_of_range("This index is out of range.");

    std::vector<bool> visited(numVertices, false);
    return depthFirstTraversalHelper(startVertex, visited);
}

std::vector<size_t> Graph::breadthFirstTraversal(size_t startVertex) const
{
    if (!validVertex(startVertex))
        throw std::out_of_range("This index is out of range.");

    std::vector<bool> visited(numVertices, false);
    std::queue<size_t> toTraverse;
    toTraverse.push(startVertex);
    visited[startVertex] = true;

    std::vector<size_t> traversal = breadthFirstTraversalHelper(visited, toTraverse);
    return traversal;
}

void Graph::clear()
{
    numVertices = 0;
    adjacencyMatrix.clear();
}

std::string Graph::toString() const
{
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
        for (size_t col : adjacencyMatrix[row])
            ss << col << " ";
        ss << "\n";
    }

    return ss.str();
}

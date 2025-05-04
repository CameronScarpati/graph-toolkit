#include "../include/Graph.h"

bool Graph::validVertex(size_t vertex) const noexcept
{
    return vertex >= 0 && vertex < numVertices;
}

std::vector<int> Graph::depthFirstTraversalHelper(
    size_t startVertex, std::vector<bool>& visited) const
{
    std::vector<int> result;
    std::stack<int> toTraverse;

    toTraverse.push(startVertex);

    while (!toTraverse.empty()) {
        size_t currentVertex = toTraverse.top();
        toTraverse.pop();

        if (!visited[currentVertex]) {
            visited[currentVertex] = true;
            result.push_back(currentVertex);
            std::vector<int> neighbors = getNeighbors(currentVertex);

            for (size_t i = neighbors.size(); i-- > 0;) {
                int neighbor = neighbors[i];
                if (!visited[neighbor])
                    toTraverse.push(neighbor);
            }
        }
    }

    return result;
}

void Graph::findHamiltonianCyclesHelper(size_t startVertex, size_t currentVertex,
    std::vector<int>& path, std::vector<bool>& visited, std::vector<std::vector<int>>& cycles) const
{
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

std::vector<int> Graph::breadthFirstTraversalHelper(
    std::vector<bool>& visited, std::queue<int>& toTraverse) const
{
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

Graph::Graph()
    : Graph(0)
{
}

Graph::Graph(size_t vertices)
    : numVertices(vertices)
    , isWeighted(false)
{
    adjacencyMatrix.resize(vertices, std::vector<int>(vertices, 0));
}

Graph::Graph(size_t vertices, bool weighted)
    : numVertices(vertices)
    , isWeighted(weighted)
{
    adjacencyMatrix.resize(vertices, std::vector<int>(vertices, 0));
}

Graph::Graph(const Graph& other)
{
    numVertices = other.numVertices;
    isWeighted = other.isWeighted;
    adjacencyMatrix = other.adjacencyMatrix;
}

Graph& Graph::operator=(const Graph& other)
{
    if (this != &other) {
        numVertices = other.numVertices;
        isWeighted = other.isWeighted;
        adjacencyMatrix = other.adjacencyMatrix;
    }
    return *this;
}

Graph::Graph(Graph&& other) noexcept
{
    numVertices = other.numVertices;
    isWeighted = other.isWeighted;
    adjacencyMatrix = std::move(other.adjacencyMatrix);

    other.numVertices = 0;
    other.clear();
}

Graph& Graph::operator=(Graph&& other) noexcept
{
    if (this != &other) {
        numVertices = other.numVertices;
        isWeighted = other.isWeighted;
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

void Graph::addEdge(size_t from, size_t to, int weight)
{
    if (!validVertex(from) || !validVertex(to))
        throw std::out_of_range("One of these indices is out of range.");

    if (weight <= 0)
        throw std::invalid_argument("Weight cannot be less than or equal zero.");

    adjacencyMatrix[from][to] = weight;
}

void Graph::addUndirectedEdge(size_t from, size_t to, int weight)
{
    addEdge(from, to, weight);
    addEdge(to, from, weight);
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

    return adjacencyMatrix[v1][v2] != 0;
}

size_t Graph::getNumVertices() const
{
    return numVertices;
}

bool Graph::getIsWeighted() const
{
    return isWeighted;
}

int Graph::getEdgeWeight(size_t from, size_t to) const
{
    if (!isAdjacent(from, to)) {
        throw std::invalid_argument("These vertices are not adjacent.");
    }

    return adjacencyMatrix[from][to];
}

std::vector<int> Graph::getNeighbors(size_t vertex) const
{
    if (!validVertex(vertex))
        throw std::out_of_range("This index is out of range.");

    std::vector<int> neighbors;
    for (size_t i = 0; i < numVertices; ++i)
        if (adjacencyMatrix[vertex][i] != 0)
            neighbors.push_back(i);

    return neighbors;
}

size_t Graph::getDegree(size_t vertex) const
{
    if (!validVertex(vertex))
        throw std::out_of_range("This index is out of range.");

    int count = 0;
    std::for_each(adjacencyMatrix[vertex].begin(), adjacencyMatrix[vertex].end(), [&count](int e) {
        if (e)
            ++count;
    });

    return count;
}

bool Graph::isConnected() const
{
    for (size_t i = 0; i < numVertices; i++) {
        std::vector<int> traversal = depthFirstTraversal(i);

        if (traversal.size() == numVertices)
            return true;
    }
    return false;
}

bool Graph::isStronglyConnected() const
{
    for (size_t i = 0; i < numVertices; i++) {
        std::vector<int> traversal = depthFirstTraversal(i);

        if (traversal.size() < numVertices)
            return false;
    }
    return true;
}

bool Graph::areVerticesStronglyConnected(size_t u, size_t v) const
{
    if (!validVertex(u) || !validVertex(v))
        throw std::out_of_range("One of these indices is out of range.");

    // Check if there's a path from u to v
    std::vector<int> fromU = depthFirstTraversal(u);
    bool uToV = std::find(fromU.begin(), fromU.end(), v) != fromU.end();

    if (!uToV)
        return false;

    // Check if there's a path from v to u
    std::vector<int> fromV = depthFirstTraversal(v);
    bool vToU = std::find(fromV.begin(), fromV.end(), u) != fromV.end();

    return vToU;
}

bool Graph::hasCycle() const
{
    std::vector<int> inDegree(getNumVertices(), 0);
    std::queue<int> toTraverse;

    size_t visited = 0;

    // Calculate in-degree of each vertex.
    for (size_t vertex = 0; vertex < getNumVertices(); ++vertex)
        for (size_t edge = 0; edge < getNumVertices(); ++edge)
            if (adjacencyMatrix[vertex][edge] != 0)
                ++inDegree[edge];

    // Add all zero in-degree vertices to a queue.
    for (size_t u = 0; u < getNumVertices(); u++)
        if (inDegree[u] == 0)
            toTraverse.push(u);

    // Run a BFS on all zero in-degree vertices.
    while (!toTraverse.empty()) {
        int sourceVertex = toTraverse.front();
        toTraverse.pop();
        visited++;

        for (size_t edge = 0; edge < numVertices; ++edge) {
            if (adjacencyMatrix[sourceVertex][edge] != 0) {
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

std::vector<std::vector<int>> Graph::findHamiltonianCycles() const
{
    if (numVertices == 1 && isAdjacent(0, 0))
        return { { 0, 0 } };
    else if (numVertices == 1)
        return {};
    if (!isConnected())
        return {};
    if (!hasCycle())
        return {};

    std::vector<std::vector<int>> hamiltonianCycles;

    for (size_t startVertex = 0; startVertex < numVertices; ++startVertex) {
        std::vector<bool> visited(numVertices, false);
        std::vector<int> path;

        visited[startVertex] = true;
        path.push_back(startVertex);

        std::vector<int> neighbors = getNeighbors(startVertex);
        for (int neighbor : neighbors) {
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

Graph Graph::minimumSpanningTree() const
{
    if (numVertices == 0)
        return {};
    if (!isConnected())
        throw std::runtime_error("MST requires a connected graph.");
    if (!isWeighted)
        throw std::runtime_error("MST algorithm requires a weighted graph.");

    Graph mst(numVertices, true); // Create a new graph to represent the MST
    std::vector<bool> inMST(numVertices, false); // Track vertices added to the MST
    std::vector<int> key(numVertices,
        std::numeric_limits<int>::max()); // Track the minimum-edge weight to each vertex
    std::vector<int> parent(numVertices, -1); // Track the parent of each vertex in the MST
    key[0] = 0;

    for (size_t count = 0; count < numVertices; ++count) {
        // Find vertex with minimum key value from the set of vertices not yet included in MST
        int minKey = std::numeric_limits<int>::max();
        int u = -1;

        for (size_t v = 0; v < numVertices; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        inMST[u] = true;

        if (parent[u] != -1) // Add edge to MST if it's not the starting vertex
            mst.addUndirectedEdge(parent[u], u, adjacencyMatrix[parent[u]][u]);

        // Update key values and parent indices of adjacent vertices
        for (size_t v = 0; v < numVertices; ++v) {
            // Update only if:
            // 1. There's an edge from u to v
            // 2. V is not in MST
            // 3. The Weight of u-v is smaller than the current key value of v
            if (adjacencyMatrix[u][v] > 0 && !inMST[v] && adjacencyMatrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = adjacencyMatrix[u][v];
            }
        }
    }

    return mst;
}

std::pair<std::vector<int>, int> Graph::travelingSalesman() const
{
    if (!this->isComplete())
        throw std::invalid_argument("The graph is not fully connected.");
    if (numVertices < 2)
        throw std::invalid_argument("TSP requires at least 2 vertices.");
    if (numVertices == 2) {
        std::vector<int> path = { 0, 1, 0 };
        int totalDistance = adjacencyMatrix[0][1] + adjacencyMatrix[1][0];
        return { path, totalDistance };
    }

    std::vector<int> bestPath;
    int bestDistance = std::numeric_limits<int>::max();

    // Generate all possible permutations of vertices (excluding the start vertex)
    std::vector<int> vertices;
    for (size_t i = 1; i < numVertices; ++i)
        vertices.push_back(static_cast<int>(i));

    do {
        // Create a complete path starting and ending at vertex 0
        std::vector<int> currentPath = { 0 };
        for (int v : vertices)
            currentPath.push_back(v);
        currentPath.push_back(0);

        // Calculate the total distance of this path
        int currentDistance = 0;
        for (size_t i = 0; i < currentPath.size() - 1; ++i) {
            int from = currentPath[i];
            int to = currentPath[i + 1];
            currentDistance += adjacencyMatrix[from][to];
        }

        // Update the best solution if this path is better
        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestPath = currentPath;
        }

    } while (std::next_permutation(vertices.begin(), vertices.end()));

    return { bestPath, bestDistance };
}

std::vector<int> Graph::depthFirstTraversal(size_t startVertex) const
{
    if (!validVertex(startVertex))
        throw std::out_of_range("This index is out of range.");

    std::vector<bool> visited(numVertices, false);
    return depthFirstTraversalHelper(startVertex, visited);
}

std::vector<int> Graph::breadthFirstTraversal(size_t startVertex) const
{
    if (!validVertex(startVertex))
        throw std::out_of_range("This index is out of range.");

    std::vector<bool> visited(numVertices, false);
    std::queue<int> toTraverse;
    toTraverse.push(startVertex);
    visited[startVertex] = true;

    std::vector<int> traversal = breadthFirstTraversalHelper(visited, toTraverse);
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
        for (int col : adjacencyMatrix[row])
            ss << col << " ";
        ss << "\n";
    }

    return ss.str();
}

void Graph::print() const
{
    std::cout << toString();
}
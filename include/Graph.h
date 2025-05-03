#ifndef HAMILTONIAN_CYCLE_FINDER_GRAPH_H
#define HAMILTONIAN_CYCLE_FINDER_GRAPH_H

#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

class Graph {
private:
    size_t numVertices;
    bool isWeighted;
    std::vector<std::vector<int>> adjacencyMatrix;

    /**
     * @brief Checks if a vertex index is valid for this graph.
     * @param vertex Vertex index to check.
     * @return true if vertex is within valid range, false otherwise.
     */
    bool validVertex(size_t vertex) const noexcept;

    /**
     * @brief Helper function for depth-first traversal.
     * @param startVertex Starting vertex for the traversal.
     * @param visited Vector tracking visited vertices.
     * @return Vector containing vertices in DFS order.
     */
    std::vector<int> depthFirstTraversalHelper(
        size_t startVertex, std::vector<bool>& visited) const;

    /**
     * @brief Helper function for breadth-first traversal.
     * @param visited Vector tracking visited vertices.
     * @param toTraverse Queue of vertices to process.
     * @return Vector containing vertices in BFS order.
     */
    std::vector<int> breadthFirstTraversalHelper(
        std::vector<bool>& visited, std::queue<int>& toTraverse) const;

    /**
     * @brief Helper function for finding hamiltonian cycles.
     * @param startVertex Starting vertex for the traversal.
     * @param currentVertex The current vertex we are exploring.
     * @param path The current path we have traversed.
     * @param visited A vector storing the vertices we have visited.
     * @param cycles A vector containing the completed hamiltonian cycles.
     */
    void findHamiltonianCyclesHelper(size_t startVertex, size_t currentVertex,
        std::vector<int>& path, std::vector<bool>& visited,
        std::vector<std::vector<int>>& cycles) const;

public:
    /**
     * @brief Default constructor, creates an empty graph.
     */
    Graph();

    /**
     * @brief Constructor creating a graph with specified number of vertices.
     * @param vertices Number of vertices to initialize.
     */
    explicit Graph(size_t vertices);

    /**
     * @brief Constructor creating a graph with specified number of vertices.
     * @param vertices Number of vertices to initialize.
     * @param weighted Is a weighted graph or not.
     */
    explicit Graph(size_t vertices, bool weighted);


    /**
     * @brief Copy constructor.
     * @param other Graph to copy.
     */
    Graph(const Graph& other);

    /**
     * @brief Copy assignment operator.
     * @param other Graph to copy.
     * @return Reference to this graph.
     */
    Graph& operator=(const Graph& other);

    /**
     * @brief Move constructor.
     * @param other Graph to move from.
     */
    Graph(Graph&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other Graph to move from.
     * @return Reference to this graph.
     */
    Graph& operator=(Graph&& other) noexcept;

    /**
     * @brief Destructor.
     */
    ~Graph();

    /**
     * @brief Adds a new vertex to the graph.
     */
    void addVertex();

    /**
     * @brief Removes a vertex from the graph.
     * @param vertex Index of vertex to remove.
     */
    void removeVertex(size_t vertex);

    /**
     * @brief Adds a directed edge between two vertices.
     * @param from Source vertex.
     * @param to Destination vertex.
     */
    void addEdge(size_t from, size_t to);

    /**
     * @brief Adds a directed edge between two vertices.
     * @param from Source vertex.
     * @param to Destination vertex.
     */
    void addEdge(size_t from, size_t to, int weight);

    /**
     * @brief Removes a directed edge between two vertices.
     * @param from Source vertex.
     * @param to Destination vertex.
     */
    void removeEdge(size_t from, size_t to);

    /**
     * @brief Checks if there's an edge from v1 to v2.
     * @param v1 Source vertex.
     * @param v2 Destination vertex.
     * @return true if v1 is adjacent to v2.
     */
    bool isAdjacent(size_t v1, size_t v2) const;

    /**
     * @brief Gets the number of vertices in the graph.
     * @return Number of vertices.
     */
    size_t getNumVertices() const;

    /**
     * @brief Gets if the graph is weighted or not.
     * @return Boolean value of if the graph is weighted.
     */
    bool getIsWeighted() const;

    /**
     * @brief Gets all adjacent vertices of a vertex.
     * @param vertex Source vertex.
     * @return Vector of adjacent vertex indices.
     */
    std::vector<int> getNeighbors(size_t vertex) const;

    /**
     * @brief Gets the out-degree of a vertex.
     * @param vertex Vertex to check.
     * @return Number of outgoing edges.
     */
    size_t getDegree(size_t vertex) const;

    /**
     * @brief Checks if the graph is connected.
     * @return true if graph is connected.
     */
    bool isConnected() const;

    /**
     * @brief Checks if the graph is strongly connected.
     * @return true if graph is strongly connected.
     */
    bool isStronglyConnected() const;

    /**
    * @brief Checks if two vertices are strongly connected in the directed graph.
     * @param u The first vertex.
     * @param v The second vertex.
     * @return True if there is a path from u to v and a path from v to u; otherwise, false.
     */
    bool areVerticesStronglyConnected(size_t u, size_t v) const;

    /**
     * @brief Checks if the graph contains a cycle.
     * @return true if graph has a cycle.
     */
    bool hasCycle() const;

    /**
     * @brief Checks if the graph is complete.
     * @return true if graph is complete.
     */
    bool isComplete() const;

    /**
     * @brief Finds all Hamiltonian cycles in the graph if they exist.
     * @return Vector of vector vertices forming a Hamiltonian cycle, empty if none exists.
     */
    std::vector<std::vector<int>> findHamiltonianCycles() const;

    /**
     * @brief Checks if the graph has a Hamiltonian cycle.
     * @return true if a Hamiltonian cycle exists.
     */
    bool hasHamiltonianCycle() const;

    /**
     * @brief Performs a depth-first traversal starting from a vertex.
     * @param startVertex Starting vertex for traversal.
     * @return Vector of vertices in DFS order.
     */
    std::vector<int> depthFirstTraversal(size_t startVertex) const;

    /**
     * @brief Performs a breadth-first traversal starting from a vertex.
     * @param startVertex Starting vertex for traversal.
     * @return Vector of vertices in BFS order.
     */
    std::vector<int> breadthFirstTraversal(size_t startVertex) const;

    /**
     * @brief Clears all vertices and edges from the graph.
     */
    void clear();

    /**
     * @brief Converts the graph to a string representation.
     * @return String representing the adjacency matrix.
     */
    std::string toString() const;

    /**
     * @brief Prints the graph's adjacency matrix to stdout.
     */
    void print() const;
};

#endif // HAMILTONIAN_CYCLE_FINDER_GRAPH_H

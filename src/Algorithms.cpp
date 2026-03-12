#include "Algorithms.h"
#include <limits>
#include <queue>
#include <stdexcept>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph& graph, size_t source)
{
    size_t n = graph.getNumVertices();

    if (source >= n)
        throw std::out_of_range("Source vertex is out of range.");

    // Check for negative weights.
    for (size_t u = 0; u < n; ++u) {
        std::vector<int> neighbors = graph.getNeighbors(u);
        for (int v : neighbors) {
            if (graph.getEdgeWeight(u, static_cast<size_t>(v)) < 0)
                throw std::invalid_argument("Graph contains negative edge weights.");
        }
    }

    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    std::vector<int> pred(n, -1);
    dist[source] = 0;

    // Min-heap: {distance, vertex}
    using PQEntry = std::pair<int, size_t>;
    std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>> pq;
    pq.push({ 0, source });

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u])
            continue;

        std::vector<int> neighbors = graph.getNeighbors(u);
        for (int neighbor : neighbors) {
            size_t v = static_cast<size_t>(neighbor);
            int weight = graph.getEdgeWeight(u, v);
            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pred[v] = static_cast<int>(u);
                pq.push({ dist[v], v });
            }
        }
    }

    return { dist, pred };
}

std::pair<std::vector<int>, std::vector<int>> bellmanFord(const Graph& graph, size_t source)
{
    size_t n = graph.getNumVertices();

    if (source >= n)
        throw std::out_of_range("Source vertex is out of range.");

    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    std::vector<int> pred(n, -1);
    dist[source] = 0;

    // Relax all edges (n - 1) times.
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t u = 0; u < n; ++u) {
            if (dist[u] == INF)
                continue;

            std::vector<int> neighbors = graph.getNeighbors(u);
            for (int neighbor : neighbors) {
                size_t v = static_cast<size_t>(neighbor);
                int weight = graph.getEdgeWeight(u, v);
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pred[v] = static_cast<int>(u);
                }
            }
        }
    }

    // Check for negative cycles.
    for (size_t u = 0; u < n; ++u) {
        if (dist[u] == INF)
            continue;

        std::vector<int> neighbors = graph.getNeighbors(u);
        for (int neighbor : neighbors) {
            size_t v = static_cast<size_t>(neighbor);
            int weight = graph.getEdgeWeight(u, v);
            if (dist[u] + weight < dist[v])
                throw std::runtime_error("Graph contains a negative cycle.");
        }
    }

    return { dist, pred };
}

std::vector<int> topologicalSort(const Graph& graph)
{
    size_t n = graph.getNumVertices();
    std::vector<int> inDegree(n, 0);
    std::queue<int> toTraverse;
    std::vector<int> result;

    // Calculate in-degree of each vertex.
    for (size_t vertex = 0; vertex < n; ++vertex) {
        std::vector<int> neighbors = graph.getNeighbors(vertex);
        for (int neighbor : neighbors)
            ++inDegree[static_cast<size_t>(neighbor)];
    }

    // Add all zero in-degree vertices to a queue.
    for (size_t u = 0; u < n; ++u)
        if (inDegree[u] == 0)
            toTraverse.push(static_cast<int>(u));

    // Process vertices in topological order.
    while (!toTraverse.empty()) {
        int sourceVertex = toTraverse.front();
        toTraverse.pop();
        result.push_back(sourceVertex);

        std::vector<int> neighbors = graph.getNeighbors(static_cast<size_t>(sourceVertex));
        for (int neighbor : neighbors) {
            --inDegree[static_cast<size_t>(neighbor)];
            if (inDegree[static_cast<size_t>(neighbor)] == 0)
                toTraverse.push(neighbor);
        }
    }

    if (result.size() != n)
        throw std::runtime_error("Graph contains a cycle.");

    return result;
}

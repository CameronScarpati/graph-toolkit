#ifndef GRAPH_TOOLKIT_ALGORITHMS_H
#define GRAPH_TOOLKIT_ALGORITHMS_H

#include "Graph.h"
#include <utility>
#include <vector>

/**
 * @brief Computes shortest paths from a source vertex using Dijkstra's algorithm.
 * @param graph The input graph (must have non-negative weights).
 * @param source The source vertex.
 * @return A pair of {distances, predecessors} from the source vertex.
 * @throws std::invalid_argument if the graph has negative weights.
 *
 * @note Uses std::priority_queue for O(E log V) complexity.
 */
std::pair<std::vector<int>, std::vector<int>> dijkstra(const Graph& graph, size_t source);

/**
 * @brief Computes shortest paths from a source vertex using Bellman-Ford algorithm.
 * @param graph The input graph.
 * @param source The source vertex.
 * @return A pair of {distances, predecessors} from the source vertex.
 * @throws std::runtime_error if a negative cycle is detected.
 *
 * @note Supports negative edge weights.
 */
std::pair<std::vector<int>, std::vector<int>> bellmanFord(const Graph& graph, size_t source);

/**
 * @brief Returns vertices in topological order using Kahn's algorithm.
 * @param graph The input directed acyclic graph.
 * @return Vector of vertex indices in topological order.
 * @throws std::runtime_error if the graph contains a cycle.
 */
std::vector<int> topologicalSort(const Graph& graph);

#endif // GRAPH_TOOLKIT_ALGORITHMS_H

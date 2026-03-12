# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2025-05-05

### Added

- Graph class with adjacency matrix representation
- Dynamic vertex addition and removal
- Directed and undirected edge support with optional weights
- Depth-first traversal (DFS) using iterative stack-based approach
- Breadth-first traversal (BFS) using queue-based approach
- Connectivity checking (`isConnected`)
- Strong connectivity checking (`isStronglyConnected`, `areVerticesStronglyConnected`)
- Cycle detection using topological sort (Kahn's algorithm)
- Completeness checking (`isComplete`)
- Hamiltonian cycle detection and enumeration via backtracking
- Minimum Spanning Tree using Prim's algorithm
- Traveling Salesman Problem solver (brute-force permutation)
- Copy and move semantics (Rule of Five)
- Adjacency matrix string representation and printing
- Google Test suite with stress tests and performance benchmarks
- GitHub Actions CI with Clang and GCC compiler matrix
- Clang-format enforcement via CI linter job

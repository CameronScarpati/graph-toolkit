<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![project_license][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<br />
<div>
  <h3>C++ Graph Toolkit</h3>

  <p>
    A robust C++ graph library designed to detect Hamiltonian cycles and perform comprehensive graph operations.
    <br />
    <a href="https://github.com/CameronScarpati/graph-toolkit"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/CameronScarpati/graph-toolkit">View Demo</a>
    ·
    <a href="https://github.com/CameronScarpati/graph-toolkit/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    ·
    <a href="https://github.com/CameronScarpati/graph-toolkit/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>

## About The Project

This repository features a robust C++ graph library built using an adjacency matrix, implementing essential graph operations, and advanced algorithms. It provides functionality for adding and removing vertices and edges, performing depth-first and breadth-first traversals, checking connectivity, detecting cycles, and finding Hamiltonian cycles.

Key features:

- Graph representation using an adjacency matrix
- Dynamic addition and removal of vertices and edges
- Depth-first and breadth-first traversal algorithms
- Detection of connectivity, cycles, and complete graphs
- Hamiltonian cycle detection for complex network analysis

### Built With

* [![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://cplusplus.com/)
* [![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)

<p>(<a href="#readme-top">back to top</a>)</p>

## Getting Started

To set up the project locally, follow these steps.

### Prerequisites

Ensure you have the following installed:

* **C++ Compiler**: Clang 14+ or GCC 12+ with C++20 support.
* **CMake**: Version 3.27 or newer.
  ```sh
  # For Ubuntu
  sudo apt-get update
  sudo apt-get install -y cmake

  # For macOS
  brew install cmake
  ```

### Installation

1. **Clone the repository**:
   ```sh
   git clone https://github.com/CameronScarpati/graph-toolkit.git
   cd graph-toolkit
   ```

2. **Configure and build**:
   ```sh
   cmake -B build -S .
   cmake --build build
   ```

<p>(<a href="#readme-top">back to top</a>)</p>

## Usage

After building, run the test suite:

```sh
cd build && ctest --output-on-failure
```

Or run the test binary directly:

```sh
./bin/testing
```

For the full API reference, see [`docs/API.md`](docs/API.md).

The test suite covers:

- Graph creation, copy, and move semantics
- Depth-first and breadth-first traversals
- Cycle detection and connectivity checks
- Hamiltonian cycle detection and enumeration
- Minimum Spanning Tree (Prim's algorithm)
- Traveling Salesman Problem (brute-force)

<p>(<a href="#readme-top">back to top</a>)</p>

## Roadmap

- [x] Implement basic graph operations (addition/removal of vertices and edges)
- [x] Develop traversal algorithms (DFS and BFS)
- [x] Implement connectivity and cycle detection
- [x] Integrate Hamiltonian cycle detection
- [x] Add weighted and undirected edge support
- [x] Implement Minimum Spanning Tree (Prim's algorithm)
- [x] Implement Traveling Salesman Problem solver
- [ ] Add Dijkstra's shortest path algorithm
- [ ] Add topological sort (public API)
- [ ] Optimize performance for large graphs

See the [open issues](https://github.com/CameronScarpati/graph-toolkit/issues) for a full list of proposed features and known issues.

<p>(<a href="#readme-top">back to top</a>)</p>

## Contributing

Contributions are welcome! Please see [`CONTRIBUTING.md`](CONTRIBUTING.md) for guidelines on development setup, code style, and the pull request process.

<p>(<a href="#readme-top">back to top</a>)</p>

## License

Distributed under the MIT License. See `LICENSE` for more information.

<p>(<a href="#readme-top">back to top</a>)</p>

## Contact

Cameron Scarpati - [LinkedIn](https://linkedin.com/in/cameron-scarpati) - cameronscarp@gmail.com

Project
Link: [https://github.com/CameronScarpati/graph-toolkit](https://github.com/CameronScarpati/graph-toolkit)

<p>(<a href="#readme-top">back to top</a>)</p>

## Acknowledgments

* [NP-Complete Problems](https://en.wikipedia.org/wiki/List_of_NP-complete_problems)
* [Hamiltonian Cycle Problem](https://en.wikipedia.org/wiki/Hamiltonian_path)
* [C++ Standard Library](https://en.cppreference.com/w/)
* [CMake Documentation](https://cmake.org/documentation/)
* [Graph Theory Resources](https://www.graph-theory.com/)

<p>(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/CameronScarpati/graph-toolkit.svg?style=for-the-badge

[contributors-url]: https://github.com/CameronScarpati/graph-toolkit/graphs/contributors

[forks-shield]: https://img.shields.io/github/forks/CameronScarpati/graph-toolkit.svg?style=for-the-badge

[forks-url]: https://github.com/CameronScarpati/graph-toolkit/network/members

[stars-shield]: https://img.shields.io/github/stars/CameronScarpati/graph-toolkit.svg?style=for-the-badge

[stars-url]: https://github.com/CameronScarpati/graph-toolkit/stargazers

[issues-shield]: https://img.shields.io/github/issues/CameronScarpati/graph-toolkit.svg?style=for-the-badge

[issues-url]: https://github.com/CameronScarpati/graph-toolkit/issues

[license-shield]: https://img.shields.io/github/license/CameronScarpati/graph-toolkit?style=for-the-badge

[license-url]: https://github.com/CameronScarpati/graph-toolkit/blob/main/LICENSE

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555

[linkedin-url]: https://linkedin.com/in/cameron-scarpati
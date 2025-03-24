cmake_minimum_required(VERSION 3.27)
project(hamiltonian_cycle_finder)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(hamiltonian_cycle_finder
        Graph.cpp
        Graph.h
        main.cpp
)

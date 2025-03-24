cmake_minimum_required(VERSION 3.27)
project(graph-toolkit)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(graph-toolkit
        Graph.cpp
        Graph.h
        main.cpp
)

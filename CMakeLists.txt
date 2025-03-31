cmake_minimum_required(VERSION 3.27)
project(graph-toolkit)

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors -g")

# Define all testing related content here
enable_testing()

# Include FetchContent module
include(FetchContent)

# Download and build Google Test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Include project headers
include_directories(./include)

# Define the source files and dependencies for the executable
set(SOURCE_FILES
        src/Graph.cpp
        tests/main.cpp
        tests/graph_test.cpp
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Create CI testing application
add_executable(testing ${SOURCE_FILES})

# Link against GTest
target_link_libraries(testing PRIVATE gtest gtest_main)
cmake_minimum_required(VERSION 3.27)
project(graph-toolkit)

# Set compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors -g")

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Include project headers
include_directories(./include)

# Enable testing
enable_testing()

# Include FetchContent module
include(FetchContent)

# Try an earlier version of Google Test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1  # Try an earlier version
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define the source files for the executable
set(SOURCE_FILES
        src/Graph.cpp
        tests/main.cpp
        tests/graph_test.cpp
)

# Create CI testing application
add_executable(testing ${SOURCE_FILES})

# Link against GTest
target_link_libraries(testing PRIVATE gtest gtest_main)

# Add tests
include(GoogleTest)
gtest_discover_tests(testing)
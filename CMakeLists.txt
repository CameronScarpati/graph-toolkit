cmake_minimum_required(VERSION 3.27)
project(graph-toolkit VERSION 0.1.0 LANGUAGES CXX)

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

# Install directories (needed before target_include_directories)
include(GNUInstallDirs)

# Library target
add_library(graph-toolkit-lib
        src/Graph.cpp
        src/Algorithms.cpp
)
target_include_directories(graph-toolkit-lib
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/graph-toolkit>
)

# Make the project root directory the working directory when we run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Create CI testing application
add_executable(testing
        tests/main.cpp
        tests/graph_test.cpp
        tests/mst_benchmark_test.cpp
        tests/algorithms_test.cpp
)

# Link against the library and GTest
target_link_libraries(testing PRIVATE graph-toolkit-lib gtest gtest_main)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(testing)

# Install targets
install(TARGETS graph-toolkit-lib
        EXPORT graph-toolkit-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/graph-toolkit
)

install(EXPORT graph-toolkit-targets
        FILE graph-toolkit-targets.cmake
        NAMESPACE graph-toolkit::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/graph-toolkit
)

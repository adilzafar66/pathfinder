# Specify the minimum required version of CMake.
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Specify project and identify languages used.
project(cpp_project-pathfinder LANGUAGES CXX)

# Enable verbose makefiles.
set(CMAKE_VERBOSE_MAKEFILE true)

# Add header files.
include_directories(include)

# Find the required Qt libraries.
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

# Add executable for main
add_executable(main app/main.cpp)
target_link_libraries(main Qt4::QtCore Qt4::QtGui)

# Add executable for random graph generator
add_executable(random_graph_generator app/random_graph_generator.cpp)
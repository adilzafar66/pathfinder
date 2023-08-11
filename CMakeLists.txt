# Specify the minimum required version of CMake.
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Specify project and identify languages used.
project(cpp_project-pathfinder)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE true)

# Add header files.
include_directories(include)

# Find the required Qt libraries.
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

# Find the required CGAL library.
find_package(CGAL REQUIRED)

# Add executable for main
add_executable(main app/main.cpp)
target_link_libraries(main Qt4::QtCore Qt4::QtGui CGAL::CGAL)

# Add executable for random graph generator
add_executable(random_graph_generator app/random_graph_generator.cpp)

install(TARGETS main random_graph_generator DESTINATION bin)
install(DIRECTORY inputs DESTINATION bin)
install(PROGRAMS demo DESTINATION bin)

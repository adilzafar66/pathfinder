# Specify the minimum required version of CMake.
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Specify project and identify languages used.
project(SENG475-Project LANGUAGES CXX)

# Enable verbose makefiles.
set(CMAKE_VERBOSE_MAKEFILE true)

# Add header files.
include_directories(include)

# Find the required Qt libraries.
find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)

# Add program target called answer.
# add_executable(test_graph app/test_graph.cpp)
# target_link_libraries(test_graph ${SDL_LIBRARY})
# target_link_libraries(test_graph Qt4::QtCore Qt4::QtGui)

# Add program target called answer.
# add_executable(test_interface app/test_interface.cpp)
# target_link_libraries(test_interface Qt4::QtCore Qt4::QtGui)

add_executable(test_dijkstra app/test_dijkstra.cpp)
target_link_libraries(test_dijkstra Qt4::QtCore Qt4::QtGui)

# add_executable(test_file app/test_file.cpp)
# target_link_libraries(test_file Qt4::QtCore Qt4::QtGui)
# add_executable(test_cli app/test_cli.cpp)

add_executable(main app/main.cpp)
target_link_libraries(main Qt4::QtCore Qt4::QtGui)
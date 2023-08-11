#include "../include/interface/window.hpp"
#include "../include/graph/graph.hpp"
#include "../include/algorithm/dijkstra.hpp"
#include "../include/algorithm/astar.hpp"
#include "../include/parser/reader.hpp"
#include "../include/parser/writer.hpp"
#include "CGAL/Point_2.h"
#include "CGAL/Cartesian.h"
#include <QApplication>
#include <stdlib.h>
#include <iostream>

using namespace algorithm;
using namespace interface;
using namespace parser;
using namespace graph;

int main(int argc, char** argv) {
    using EdgeInfo = std::tuple<unsigned int, unsigned int, double>;
    using VertexInfo = std::tuple<unsigned int, float, float>;

    if (argc < 2)
        throw std::invalid_argument("Error: File path to write to is required.");

    GraphFileWriter<float> gfw(argv[1]);
    Graph<int> graph;
    std::vector<EdgeInfo> edges;
    std::vector<VertexInfo> vertices;

    auto start_end = std::make_tuple<unsigned int>(25, 295);
    int num_vertices = 100;
    int num_edges = 500;
    int max_bound = 100;
    int cost = 10;

    for (int i = 0; i < num_vertices; i++) {
        vertices.emplace_back(i, 1 + (rand() % max_bound), 1 + (rand() % max_bound));
    }

    for (int i = 0; i < num_edges; i++) {
        auto v1 = std::get<0>(vertices[(rand() % num_vertices)]);
        auto v2 = std::get<0>(vertices[(rand() % num_vertices)]);
        edges.emplace_back(v1, v2, (rand() % cost));
    }

    gfw.write_start_end(start_end);
    gfw.write_vertices(vertices);
    gfw.write_edges(edges);

    return 0;
}
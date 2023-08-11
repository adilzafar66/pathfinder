#include "../include/parser/reader.hpp"
#include "../include/parser/writer.hpp"
#include <stdlib.h>
#include <iostream>

using namespace parser;

int main(int argc, char** argv) {
    using EdgeInfo = std::tuple<unsigned int, unsigned int, double>;
    using VertexInfo = std::tuple<unsigned int, float, float>;

    if (argc < 2)
        throw std::invalid_argument("Error: File path to write to is required.");

    GraphFileWriter<float> gfw(argv[1]);

    std::vector<EdgeInfo> edges;
    std::vector<VertexInfo> vertices;

    int start, end;
    int num_vertices, num_edges;
    int max_bound = 100;
    int cost = 10;

    // Get num_vertices
    std::cout << "Enter the number of vertices: ";
    if (!(std::cin >> num_vertices)) {
        std::cerr << "Invalid input for num_vertices." << std::endl;
        return 1;
    }

    // Get num_edges
    std::cout << "Enter the number of edges: ";
    if (!(std::cin >> num_edges)) {
        std::cerr << "Invalid input for num_edges." << std::endl;
        return 1;
    }

    // Get start vertex
    std::cout << "Enter the start vertex: ";
    if (!(std::cin >> start || start < 0)) {
        std::cerr << "Invalid input for start." << std::endl;
        return 1;
    }

    // Get end vertex
    std::cout << "Enter the end vertex: ";
    if (!(std::cin >> end || end > num_vertices)) {
        std::cerr << "Invalid input for end." << std::endl;
        return 1;
    }

    auto start_end = std::make_tuple<unsigned int>(start, end);

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
    gfw.write_edges(edges, "");

    return 0;
}
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <stdexcept>
#include <QApplication>
#include "../include/graph/graph.hpp"
#include "../include/parser/reader.hpp"
#include "../include/parser/writer.hpp"
#include "../include/interface/window.hpp"
#include "../include/interface/interface.hpp"
#include "../include/algorithm/astar.hpp"
#include "../include/algorithm/dijkstra.hpp"

using namespace graph;
using namespace parser;
using namespace interface;

int main(int argc, char** argv) {
    try {

        CLIInterface cli(argc, argv);
        const std::string algorithm = cli.get_algorithm();
        const std::string input_file = cli.get_input_file();
        const std::string output_file = cli.get_output_file();
        const bool path_only = cli.get_path_only();

        GraphFileReader<double> gf_reader(input_file);

        unsigned int start, end;
        auto vertices = gf_reader.get_vertices();
        auto edges = gf_reader.get_edges();
        std::tie(start, end) = gf_reader.get_start_end();
        Graph<double> main_graph(vertices, edges);

        if (algorithm == "astar")
            compute_astar(main_graph, start, end);
        if (algorithm == "dijkstra")
            compute_dijkstra(main_graph, start, end);
        if (algorithm == "all") {
            compute_astar(main_graph, start, end);
            compute_dijkstra(main_graph, start, end);
        }

        if (!output_file.empty()) {
            GraphFileWriter<double> gf_writer(output_file);
            gf_writer.write_start_end(gf_reader.get_start_end());
            auto astar_path = main_graph.get_astar_path();
            auto dijkstra_path = main_graph.get_dijkstra_path();
            
            if (!astar_path.empty()){
                gf_writer.write_edges(main_graph.get_path_edge_elements(astar_path), "A*");
                auto astar_cost = main_graph.get_path_cost(astar_path);
                auto astar_distance = main_graph.get_path_distance(astar_path);
                gf_writer.write_cost_distance(astar_cost, astar_distance);
            }

            if (!dijkstra_path.empty()) {
                gf_writer.write_edges(main_graph.get_path_edge_elements(dijkstra_path), "Dijkstra");
                auto dijkstra_cost = main_graph.get_path_cost(dijkstra_path);
                auto dijkstra_distance = main_graph.get_path_distance(dijkstra_path);
                gf_writer.write_cost_distance(dijkstra_cost, dijkstra_distance);
            }
        }

        QApplication app(argc, argv);
        interface::MainWindow<double> main_window;
        main_window.set_graph(main_graph);
        main_window.draw_graph(50, 10, path_only);
        main_window.show();
        return app.exec();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
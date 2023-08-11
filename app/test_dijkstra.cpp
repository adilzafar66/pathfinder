#include "../include/interface/window.hpp"
#include "../include/graph/graph.hpp"
#include "../include/algorithm/dijkstra.hpp"
#include "../include/algorithm/astar.hpp"
#include "CGAL/Point_2.h"
#include "CGAL/Cartesian.h"
#include <QApplication>
#include <stdlib.h>
#include <iostream>

using namespace algorithm;
using namespace interface;
using namespace graph;

int main(int argc, char** argv) {
    using Point = CGAL::Point_2<CGAL::Cartesian<int>>;
    using Point_T = std::tuple<unsigned int, Point>;

    Graph<int> graph;
    std::vector<std::tuple<Point_T, Point_T, double>> data;
    std::vector<std::tuple<int, Point>> points;

    // for (int i = 0; i < 20; i++) {
    //     auto p1 = std::make_tuple(i, Point(1+ (rand() % 20), 1+ (rand() % 20)));
    //     auto p2 = std::make_tuple(i + 1, Point(1+ (rand() % 20), 1+ (rand() % 20)));
    //     points.push_back(p1);
    //     points.push_back(p2);
    // }

    // for (int i = 0; i < 20; i++) {
    //     auto edge = std::make_tuple(points[(rand() % 20)], points[(rand() % 20)], 3);
    //     data.push_back(edge);
    // }

    // for (int i = 0; i < 20; i++) {
    //     for (int j = 0; j < 20; j+=2) {
    //         auto p1 = std::make_tuple(i * 20 + j , Point(j, i));
    //         auto p2 = std::make_tuple(i * 20 + j + 1 , Point(j + 1, i));
    //         auto edge = std::make_tuple(p1, p2, 3);
    //         data.push_back(edge);
    //     }
    // }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j+=1) {
            auto p = std::make_tuple(i * 20 + j , Point(j, i));
            points.push_back(p);
        }
    }

    for (int j = 20; j < 380; j+=1) {
        auto edge3 = std::make_tuple(points[j], points[j - 20], (rand() % 20));
        auto edge4 = std::make_tuple(points[j], points[j + 20], (rand() % 20));
        data.push_back(edge4);
        data.push_back(edge3);
        if (j % 20 != 0) {
            auto edge2 = std::make_tuple(points[j], points[j-1], (rand() % 20));
            data.push_back(edge2);
        }
        if (j % 20 != 19) {
            auto edge1 = std::make_tuple(points[j], points[j+1], (rand() % 20));
            data.push_back(edge1);
        }

    }
    
    graph.create_mesh(data);
    compute_dijkstra(graph, 111, 387);
    compute_astar(graph, 111, 387);
    QApplication app(argc, argv);
    MainWindow<int> main_window;
    main_window.set_graph(graph);
    main_window.draw_graph(50);
    main_window.show();
    return app.exec();
}
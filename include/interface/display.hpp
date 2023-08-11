#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QBrush>
#include <QPointF>
#include <QToolTip>
#include <QPainter>
#include <QPolygonF>
#include <iostream>
#include <vector>
#include <cmath>
#include <QPen>
#include "../graph/graph.hpp"
#include "node.hpp"

using namespace graph;

namespace interface
{
    template <class T>
    class GraphDisplay : public QGraphicsView
    {
    public:
        GraphDisplay(QWidget *parent = nullptr);
        void set_graph(const Graph<T> &graph_);
        void set_scale_factor(double sf);
        void draw_graph(bool path_only = false);
        void zoom_in();
        void zoom_out();

    protected:
        void wheelEvent(QWheelEvent *event) override;

    private:
        Graph<T> graph;
        double scale_factor;
        QGraphicsScene scene;
        QPoint last_mouse_pos;
        void create_legend();
        std::unordered_map<unsigned int, ClickableVertexItem *> circles;
        void draw_vertex(Vertex<T> *vertex, Qt::GlobalColor color = Qt::black);
        void draw_edge(Edge<T> *edge, Qt::GlobalColor color = Qt::black, int thickness = 1, double arrow_size = 8);
        void draw_path(std::vector<unsigned int> path, Qt::GlobalColor edge_color = Qt::darkGreen);
        void show_edge_info(const QPointF &pos, const QString &info);
    };

    template <class T>
    inline GraphDisplay<T>::GraphDisplay(QWidget *parent) : QGraphicsView(parent), scale_factor(1)
    {
        setScene(&scene);
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setInteractive(true);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true); // Disable AA adjustment
        setOptimizationFlag(QGraphicsView::DontSavePainterState, true);      // Disable painter state saving
        scene.setSceneRect(-5000, -5000, 10000, 10000);                      // Example values, adjust as needed
        create_legend();
    }

    template <class T>
    inline void GraphDisplay<T>::set_graph(const Graph<T> &graph_)
    {
        graph = graph_;
    }

    template <class T>
    inline void GraphDisplay<T>::set_scale_factor(double sf)
    {
        scale_factor = sf;
    }

    template <class T>
    inline void GraphDisplay<T>::draw_graph(bool path_only)
    {
        scene.clear();
        if (!path_only)
        {
            for (auto &vertex : graph.get_vertices())
            {
                for (auto &edge : vertex.second->get_edges())
                    draw_edge(edge);
            }
        }
        for (auto &vertex : graph.get_vertices())
            draw_vertex(vertex.second);

        auto astar_path = graph.get_astar_path();
        auto dijkstra_path = graph.get_dijkstra_path();

        if (!astar_path.empty())
            draw_path(astar_path, Qt::darkGreen);

        if (!dijkstra_path.empty())
            draw_path(dijkstra_path, Qt::darkBlue);
    }

    template <class T>
    inline void GraphDisplay<T>::draw_vertex(Vertex<T> *vertex, Qt::GlobalColor color)
    {
        double x = vertex->get_x() * scale_factor;
        double y = vertex->get_y() * scale_factor;
        QPointF point(x, y);
        ClickableVertexItem *circle = new ClickableVertexItem(vertex->get_position(), color);
        circle->setPos(point);
        scene.addItem(circle);
        circles[vertex->get_position()] = circle;
    }

    template <class T>
    inline void GraphDisplay<T>::draw_edge(Edge<T> *edge, Qt::GlobalColor color, int thickness, double arrow_size)
    {
        color = edge->get_cost() < 0 ? Qt::red : color;
        double x1 = edge->get_source()->get_x() * scale_factor;
        double y1 = edge->get_source()->get_y() * scale_factor;
        double x2 = edge->get_destination()->get_x() * scale_factor;
        double y2 = edge->get_destination()->get_y() * scale_factor;
        QGraphicsLineItem *line = scene.addLine(x1, y1, x2, y2, QPen(color, thickness, Qt::SolidLine, Qt::RoundCap));

        // Calculate the direction vector
        double dx = x2 - x1;
        double dy = y2 - y1;
        double length = std::sqrt(dx * dx + dy * dy);
        double arrow_x = x2 - 8 * dx / length;
        double arrow_y = y2 - 8 * dy / length;

        double angle = std::atan2(dy, dx); // Angle of the arrow

        // Calculate the coordinates for the arrowhead triangle
        double arrow_x1 = arrow_x + arrow_size * std::cos(angle + M_PI * 5.0 / 6.0); // 150 degrees
        double arrow_y1 = arrow_y + arrow_size * std::sin(angle + M_PI * 5.0 / 6.0);
        double arrow_x2 = arrow_x + arrow_size * std::cos(angle - M_PI * 5.0 / 6.0); // -150 degrees
        double arrow_y2 = arrow_y + arrow_size * std::sin(angle - M_PI * 5.0 / 6.0);

        // Draw the arrowhead as a triangle
        QPolygonF arrowhead_polygon;
        arrowhead_polygon.append(QPointF(arrow_x, arrow_y));
        arrowhead_polygon.append(QPointF(arrow_x1, arrow_y1));
        arrowhead_polygon.append(QPointF(arrow_x2, arrow_y2));
        QGraphicsPolygonItem *arrowhead = scene.addPolygon(arrowhead_polygon, QPen(color, 1), QBrush(color));
    }

    template <class T>
    inline void GraphDisplay<T>::zoom_in()
    {
        scale(1.1, 1.1); // Increase the scale factor to zoom in
    }

    template <class T>
    inline void GraphDisplay<T>::zoom_out()
    {
        scale(1.0 / 1.1, 1.0 / 1.1); // Decrease the scale factor to zoom out
    }

    template <class T>
    inline void GraphDisplay<T>::wheelEvent(QWheelEvent *event)
    {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        if (numSteps > 0)
            zoom_in();
        else
            zoom_out();
        event->accept();
    }

    template <class T>
    inline void GraphDisplay<T>::draw_path(std::vector<unsigned int> path, Qt::GlobalColor edge_color)
    {

        auto edges = graph.get_path_edges(path);
        for (auto &edge : edges)
        {
            draw_edge(edge, edge_color, 4);
        }
        draw_vertex(graph.get_vertex(path.front()), Qt::yellow);
        for (unsigned int i = 1; i < path.size() - 1; i++)
        {
            scene.removeItem(circles[path[i]]);
            draw_vertex(graph.get_vertex(path[i]), Qt::green);
        }
        draw_vertex(graph.get_vertex(path.back()), Qt::red);
    }

    template<class T>
    void GraphDisplay<T>::create_legend() {
        // Create the legend widget
        QWidget* legend_widget = new QWidget;
        legend_widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum); // Set size policy
        QHBoxLayout* legend_layout = new QHBoxLayout(legend_widget);

        // Create the green legend label
        QLabel* green_label = new QLabel("A*");
        green_label->setStyleSheet("background-color: green; color: white; padding: 2px;");
        legend_layout->addWidget(green_label);

        // Create the blue legend label
        QLabel* blue_label = new QLabel("Dijkstra");
        blue_label->setStyleSheet("background-color: blue; color: white; padding: 2px;");
        legend_layout->addWidget(blue_label);

        // Add the legend widget to the top right corner of the layout
        QVBoxLayout* main_layout = new QVBoxLayout(this);
        main_layout->addWidget(legend_widget, 0, Qt::AlignTop | Qt::AlignRight); // Align to top right corner
        main_layout->addWidget(&scene);

        // Set the main layout for the widget
        setLayout(main_layout);
    }
} // namespace interface

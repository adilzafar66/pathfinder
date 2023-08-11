#pragma once

#include <QMainWindow>
#include "../interface/display.hpp"
#include "../graph/graph.hpp"

namespace interface
{
    template <class T>
    class MainWindow : public QMainWindow
    {
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        void set_graph(const graph::Graph<T> &graph);
        void draw_graph(unsigned int scale_factor, unsigned int num_vertices, bool path_only = true);

    private:
        GraphDisplay<T> *display;
    };

    template <class T>
    MainWindow<T>::MainWindow(QWidget *parent) : QMainWindow(parent)
    {
        display = new GraphDisplay<T>(this);
        setCentralWidget(display);
    }

    template <class T>
    void MainWindow<T>::set_graph(const graph::Graph<T> &graph)
    {
        display->set_graph(graph);
    }

    template <class T>
    void MainWindow<T>::draw_graph(unsigned int scale_factor, unsigned int num_vertices, bool path_only)
    {
        resize(800, 800);
        unsigned int size = num_vertices * 2;
        display->set_scale_factor(scale_factor);
        display->set_view();
        display->create_legend();
        display->draw_graph(path_only);
    }

} // namespace interface

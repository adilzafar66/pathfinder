#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "../interface/display.hpp"
#include "../graph/graph.hpp"

namespace interface
{
    template <class T>
    class MainWindow : public QMainWindow
    {
    public:
        // Constructor for the main window
        explicit MainWindow(QWidget *parent = nullptr);
        void set_graph(const graph::Graph<T> &graph);
        void draw_graph(unsigned int scale_factor, bool path_only = true);

    private:
        GraphDisplay<T> *display;
    };

    template <class T>
    MainWindow<T>::MainWindow(QWidget *parent) : QMainWindow(parent)
    {
        // Create and set the display widget as the central widget
        display = new GraphDisplay<T>(this);
        setCentralWidget(display);
    }

    template <class T>
    void MainWindow<T>::set_graph(const graph::Graph<T> &graph)
    {
        // Set the graph to be displayed in the widget
        display->set_graph(graph);
    }

    template <class T>
    void MainWindow<T>::draw_graph(unsigned int scale_factor, bool path_only)
    {
        // Set window size and scale factor
        resize(800, 800);
        display->set_scale_factor(scale_factor);
        display->set_view();
        display->create_legend();
        display->draw_graph(path_only);
    }
} // namespace interface

#endif // WINDOW_H

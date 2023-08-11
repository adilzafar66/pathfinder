#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <tuple>
#include <assert.h>
#include "vertex.hpp"

namespace graph
{
    template <class T>
    class Graph
    {

    public:
        using ValueType = T;
        using VertexPtr = Vertex<T> *;
        using Positions = std::vector<unsigned int>;
        using Point = typename Vertex<T>::Point;
        using Edges = typename Vertex<T>::Edges;
        using Vertices = std::unordered_map<unsigned int, VertexPtr>;
        using Visited = std::unordered_map<VertexPtr, bool>;
        using Point_T = std::tuple<unsigned int, Point>;
        using VertexElements = std::vector<std::tuple<unsigned int, T, T>>;
        using EdgeElements = std::vector<std::tuple<unsigned int, unsigned int, double>>;
        using Element = std::tuple<Point_T, Point_T, double>;
        using Elements = std::vector<Element>;
        using Bounds = std::tuple<T, T, T, T>;

        Graph();
        Graph(Vertices vertices);
        Graph(VertexElements vertex_elems, EdgeElements edge_elems);
        ~Graph();

        Bounds get_bounds();
        Vertices get_vertices();
        VertexPtr get_vertex(unsigned int position);
        std::size_t get_num_vertices() const;
        Positions get_optimal_path();
        Positions get_astar_path();
        Positions get_dijkstra_path();
        ValueType get_path_distance(const Positions& path);
        Edges get_path_edges(const Positions& path);
        EdgeElements get_path_edge_elements(const Positions& path);
        ValueType get_distance(const VertexPtr& from_vertex, const VertexPtr& to_vertex);
        double get_heuristic(unsigned int from_position, unsigned int to_position);
        double get_path_cost(const Positions& path);

        void set_vertices(Vertices vertices);
        void create_vertices(VertexElements vertex_elems);
        void create_edges(EdgeElements edge_elems);
        void add_vertex(VertexPtr vertex);
        void remove_vertex(unsigned int position);
        bool vertex_exists(unsigned int position);
        bool are_vertices_connected(unsigned int start_position, unsigned int end_position);
        bool is_empty() const;
        bool is_connected();

        void set_optimal_path(const Positions &path);
        void set_astar_path(const Positions &path);
        void set_dijkstra_path(const Positions &path);
        void dfs_recursive(VertexPtr vertex);
        void depth_first_search(VertexPtr start_vertex);

        void create_mesh(Elements elements);
        void create_mesh(Vertices vertices, EdgeElements edge_elems);
        void create_mesh(VertexElements vertex_elems, EdgeElements edge_elems);

        void add_visited(VertexPtr vertex_ptr, bool status);
        bool get_visited(VertexPtr vertex_ptr);
        void clear_visited();

        void clear_graph();
        void print_graph();

    private:
        Visited visited;
        Vertices vertices;
        Positions astar_path;
        Positions dijkstra_path;
        Positions optimal_path;
        std::size_t edge_count;
    };

    template <class T>
    inline Graph<T>::Graph() {}

    template <class T>
    inline Graph<T>::Graph(Vertices vertices)
    {
        this->vertices = vertices;
    }

    template <class T>
    inline Graph<T>::Graph(VertexElements vertex_elems, EdgeElements edge_elems)
    {
        create_mesh(vertex_elems, edge_elems);
    }

    template <class T>
    inline Graph<T>::~Graph() {}

    template <class T>
    inline typename Graph<T>::Bounds Graph<T>::get_bounds()
    {
        if (vertices.empty())
        {
            return std::make_tuple(0, 0, 0, 0); // Default bounds for an empty graph
        }

        ValueType min_x = vertices.begin()->second->get_x();
        ValueType max_x = vertices.begin()->second->get_x();
        ValueType min_y = vertices.begin()->second->get_y();
        ValueType max_y = vertices.begin()->second->get_y();

        for (const auto &vertex : vertices)
        {
            ValueType x = vertex.second->get_x();
            ValueType y = vertex.second->get_y();

            if (x < min_x)
                min_x = x;
            if (x > max_x)
                max_x = x;
            if (y < min_y)
                min_y = y;
            if (y > max_y)
                max_y = y;
        }

        ValueType width = max_x - min_x;
        ValueType height = max_y - min_y;
        return std::make_tuple(min_x, min_y, width, height);
    }

    template <class T>
    inline typename Graph<T>::Vertices Graph<T>::get_vertices()
    {
        return vertices;
    }

    template <class T>
    inline typename Graph<T>::VertexPtr Graph<T>::get_vertex(unsigned int positon)
    {
        return vertices[positon];
    }

    template <class T>
    inline void Graph<T>::set_vertices(Vertices vertices)
    {
        this->vertices = vertices;
    }

    template <class T>
    inline void Graph<T>::add_vertex(VertexPtr vertex)
    {
        unsigned int position = vertex->get_position();
        vertices.emplace(position, vertex);
    }

    template <class T>
    inline void Graph<T>::remove_vertex(unsigned int position)
    {
        vertices.erase(position);
    }

    template <class T>
    inline bool Graph<T>::vertex_exists(unsigned int position)
    {
        return vertices.find(position) != vertices.end();
    }

    template <class T>
    inline typename Graph<T>::EdgeElements Graph<T>::get_path_edge_elements(const Positions& path) {
        EdgeElements path_edges;
        for (unsigned int i = 0; i < path.size() - 1; i++){
            auto vertex = get_vertex(path[i]);
            for (auto& edge: vertex->get_edges()) {
                auto neighbor_pos = edge->get_destination()->get_position();
                if (neighbor_pos == path[i + 1])
                    path_edges.emplace_back(path[i], path[i + 1], edge->get_cost());
            }
        }
        return path_edges;
    }

    template <class T>
    inline typename Graph<T>::Edges Graph<T>::get_path_edges(const Positions& path) {
        Edges path_edges;
        for (unsigned int i = 0; i < path.size() - 1; i++){
            auto vertex = get_vertex(path[i]);
            for (auto& edge: vertex->get_edges()) {
                auto neighbor_pos = edge->get_destination()->get_position();
                if (neighbor_pos == path[i + 1])
                    path_edges.emplace_back(edge);
            }
        }
        return path_edges;
    }

    template <class T>
    inline typename Graph<T>::ValueType Graph<T>::get_distance(const VertexPtr& from_vertex, const VertexPtr& to_vertex)
    {
        double squared_dist = CGAL::squared_distance(from_vertex->get_coordinates(), to_vertex->get_coordinates());
        return static_cast<typename Graph<T>::ValueType>(std::sqrt(squared_dist));
    }

    template <class T>
    inline typename Graph<T>::ValueType Graph<T>::get_path_distance(const Positions& path)
    {
        T path_distance = 0;
        Edges edges = get_path_edges(path);
        for (auto& edge : edges) {
            path_distance += edge->get_length();
        }
        return path_distance;
    }

    template <class T>
    inline double Graph<T>::get_path_cost(const Positions& path)
    {
        double path_cost = 0;
        Edges edges = get_path_edges(path);
        for (auto& edge : edges) {
            path_cost += edge->get_cost();
        }
        return path_cost;
    }

    template<class T>
    inline double Graph<T>::get_heuristic(unsigned int from_position, unsigned int to_position)
    {
        const auto& from_vertex = get_vertex(from_position);
        const auto& to_vertex = get_vertex(to_position);
        return get_distance(from_vertex, to_vertex);
    }

    template <class T>
    inline bool Graph<T>::is_empty() const
    {
        return vertices.empty();
    }

    template <class T>
    inline std::size_t Graph<T>::get_num_vertices() const
    {
        return vertices.size();
    }

    template <class T>
    inline bool Graph<T>::is_connected()
    {
        if (vertices.empty())
        {
            return true;
        }
        for (const auto &vertex : vertices)
        {
            visited[vertex.second] = false;
        }
        auto start_vertex = vertices.begin()->second;
        dfs_recursive(start_vertex);
        for (const auto &vertex : vertices)
        {
            if (!visited[vertex.second])
            {
                return false;
            }
        }
        return true;
    }

    template <class T>
    inline bool Graph<T>::are_vertices_connected(unsigned int start_position, unsigned int end_position)
    {
        for (const auto &vertex : vertices)
        {
            visited[vertex.second] = false;
        }
        auto start_vertex = get_vertex(start_position);
        auto end_vertex = get_vertex(end_position);
        dfs_recursive(start_vertex);
        bool connected = visited[end_vertex];
        for (const auto &vertex : vertices)
        {
            visited[vertex.second] = false;
        }
        return connected;
    }

    template <class T>
    inline typename Graph<T>::Positions Graph<T>::get_optimal_path()
    {
        return optimal_path;
    }

    template <class T>
    inline typename Graph<T>::Positions Graph<T>::get_astar_path()
    {
        return astar_path;
    }

    template <class T>
    inline typename Graph<T>::Positions Graph<T>::get_dijkstra_path()
    {
        return dijkstra_path;
    }

    template <class T>
    inline void Graph<T>::set_optimal_path(const Positions &path)
    {
        optimal_path = path;
    }

    template <class T>
    inline void Graph<T>::set_astar_path(const Positions &path)
    {
        astar_path = path;
    }

        template <class T>
    inline void Graph<T>::set_dijkstra_path(const Positions &path)
    {
        dijkstra_path = path;
    }

    template <class T>
    inline void Graph<T>::dfs_recursive(VertexPtr vertex)
    {
        vertex->print_coordinates();
        visited[vertex] = true;
        for (const auto &edge : vertex->get_edges())
        {
            VertexPtr neighbor = (edge->get_source() == vertex) ? edge->get_destination() : edge->get_source();
            if (!visited[neighbor])
            {
                dfs_recursive(neighbor);
            }
        }
    }

    template <class T>
    inline void Graph<T>::depth_first_search(VertexPtr start_vertex)
    {
        for (const auto &vertex : vertices)
        {
            visited[vertex.second] = false;
        }
        dfs_recursive(start_vertex);
    }

    template <class T>
    inline void Graph<T>::clear_graph()
    {
        for (const auto &vertex : vertices)
        {
            delete vertex.second;
        }
        vertices.clear();
    }

    template <class T>
    inline void Graph<T>::create_mesh(Elements elements)
    {
        clear_graph();
        for (auto element : elements)
        {
            auto src_id = std::get<0>(element);
            auto src_pos = std::get<0>(src_id);
            auto src_coors = std::get<1>(src_id);
            auto dest_id = std::get<1>(element);
            auto dest_pos = std::get<0>(dest_id);
            auto dest_coors = std::get<1>(dest_id);
            auto edge_cost = std::get<2>(element);

            VertexPtr src_vertex = nullptr;
            auto src_it = vertices.find(src_pos);

            if (src_it != vertices.end())
            {
                src_vertex = src_it->second;
            }
            else
            {
                src_vertex = new Vertex<T>(src_pos, src_coors);
                vertices[src_pos] = src_vertex;
            }

            VertexPtr dest_vertex = nullptr;
            auto dest_it = vertices.find(dest_pos);

            if (dest_it != vertices.end())
            {
                dest_vertex = dest_it->second;
            }
            else
            {
                dest_vertex = new Vertex<T>(dest_pos, dest_coors);
                vertices[dest_pos] = dest_vertex;
            }
            auto edge = new Edge<T>(src_vertex, dest_vertex, edge_cost);
            src_vertex->add_edge(edge);
        }
    }

    template <class T>
    inline void Graph<T>::create_mesh(Vertices vertices, EdgeElements edge_elems)
    {
        set_vertices(vertices);
        create_edges(edge_elems);
    }

    template <class T>
    inline void Graph<T>::create_mesh(VertexElements vertex_elems, EdgeElements edge_elements)
    {
        create_vertices(vertex_elems);
        create_edges(edge_elements);
    }

    template <class T>
    inline void Graph<T>::add_visited(VertexPtr vertex_ptr, bool status)
    {
        visited[vertex_ptr] = status;
    }

    template <class T>
    inline void Graph<T>::clear_visited()
    {
        visited.clear();
    }

    template <class T>
    inline bool Graph<T>::get_visited(VertexPtr vertex_ptr)
    {
        return visited[vertex_ptr];
    }

    template <class T>
    inline void Graph<T>::create_vertices(VertexElements vertex_elems)
    {
        for (auto element : vertex_elems)
        {
            auto p = std::get<0>(element);
            auto x = std::get<1>(element);
            auto y = std::get<2>(element);
            Vertex<T> *vertex = new Vertex<T>(p, x, y);
            add_vertex(vertex);
        }
    }

    template <class T>
    inline void Graph<T>::create_edges(EdgeElements edge_elems)
    {
        for (auto element : edge_elems)
        {
            auto src_pos = std::get<0>(element);
            auto dest_pos = std::get<1>(element);
            auto cost = std::get<2>(element);
            auto src = get_vertex(src_pos);
            auto dest = get_vertex(dest_pos);
            auto edge = new Edge<T>(src, dest, cost);
            src->add_edge(edge);
        }
    }

    template <class T>
    inline void Graph<T>::print_graph()
    {
        for (auto vertex : vertices)
        {
            vertex.second->print_position();
            vertex.second->print_coordinates();
            vertex.second->print_edges();
            std::cout << std::endl;
        }
    }
} // namespace graph

#endif // GRAPH_H
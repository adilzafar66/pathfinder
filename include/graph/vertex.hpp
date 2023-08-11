#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>
#include "edge.hpp"
#include "CGAL/Point_2.h"
#include "CGAL/Cartesian.h"

namespace graph
{
    template <class T>
    class Vertex
    {
    public:
        using ValueType = T;
        using EdgePtr = Edge<T> *;
        using Edges = std::vector<Edge<T> *>;
        using Point = CGAL::Point_2<CGAL::Cartesian<T>>;

        Vertex(unsigned int position);
        Vertex(unsigned int position, ValueType x, ValueType y);
        Vertex(unsigned int position, ValueType x, ValueType y, const Edges &edges);
        Vertex(unsigned int position, Point coordinates);
        Vertex(unsigned int position, Point coordinates, const Edges &edges);
        ~Vertex();

        // Getters
        unsigned int get_position() const;
        Point get_coordinates() const;
        ValueType get_x() const;
        ValueType get_y() const;
        Edges get_edges() const;

        // Setters and modifiers
        void set_edges(const Edges &edges);
        void add_edge(EdgePtr edge);
        void remove_edge(EdgePtr edge);
        void clear_edges();

        // Connectivity and neighbor information
        bool is_connected_to_edge(const EdgePtr edge);
        std::size_t get_num_edges() const;
        std::vector<Vertex<T> *> get_neighboring_vertices() const;
        ValueType get_min_edge_cost() const;
        ValueType get_max_edge_cost();
        std::string to_string() const;

        // Debugging and printing
        void print_coordinates() const;
        void print_edges() const;
        void print_position() const;

    private:
        unsigned int position;
        Point coordinates;
        Edges edges;
    };

    // Constructor implementations
    template <class T>
    inline Vertex<T>::Vertex(unsigned int position) : position(position), coordinates(Point(0, 0)) {}

    template <class T>
    inline Vertex<T>::Vertex(unsigned int position, ValueType x, ValueType y) : position(position)
    {
        coordinates = Point(x, y);
    }

    template <class T>
    inline Vertex<T>::Vertex(unsigned int position, ValueType x, ValueType y, const Edges &edges) : position(position), edges(edges)
    {
        coordinates = Point(x, y);
    }

    template <class T>
    inline Vertex<T>::Vertex(unsigned int position, Point coordinates) : position(position), coordinates(coordinates) {}

    template <class T>
    inline Vertex<T>::Vertex(unsigned int position, Point coordinates, const Edges &edges) : position(position), coordinates(coordinates), edges(edges) {}

    template <class T>
    inline Vertex<T>::~Vertex() {}

    template <class T>
    inline unsigned int Vertex<T>::get_position() const
    {
        return position;
    }

    template <class T>
    inline typename Vertex<T>::Point Vertex<T>::get_coordinates() const
    {
        return coordinates;
    }

    template <class T>
    inline typename Vertex<T>::ValueType Vertex<T>::get_x() const
    {
        return coordinates.x();
    }

    template <class T>
    inline typename Vertex<T>::ValueType Vertex<T>::get_y() const
    {
        return coordinates.y();
    }

    template <class T>
    inline typename Vertex<T>::Edges Vertex<T>::get_edges() const
    {
        return edges;
    }

    template <class T>
    inline void Vertex<T>::set_edges(const Edges &edges_)
    {
        edges = edges_;
    }

    template <class T>
    inline void Vertex<T>::add_edge(EdgePtr edge)
    {
        edges.push_back(edge);
    }

    template <class T>
    inline void Vertex<T>::remove_edge(EdgePtr edge)
    {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }

    template <class T>
    inline void Vertex<T>::clear_edges()
    {
        edges.clear();
    }

    // Other method implementations
    template <class T>
    inline bool Vertex<T>::is_connected_to_edge(const EdgePtr edge)
    {
        return std::find(edges.begin(), edges.end(), edge) != edges.end();
    }

    template <class T>
    inline std::size_t Vertex<T>::get_num_edges() const
    {
        return edges.size();
    }

    template <class T>
    inline std::vector<Vertex<T> *> Vertex<T>::get_neighboring_vertices() const
    {
        std::vector<Vertex<T> *> neighbors;
        for (const auto &edge : edges)
        {
            if (edge->get_source() == this)
            {
                neighbors.push_back(edge->get_destination());
            }
            else
            {
                neighbors.push_back(edge->get_source());
            }
        }
        return neighbors;
    }

    template <class T>
    inline typename Vertex<T>::ValueType Vertex<T>::get_min_edge_cost() const
    {
        T min_cost = std::numeric_limits<T>::max();
        for (const auto &edge : edges)
        {
            min_cost = std::min(min_cost, edge->get_cost());
        }
        return min_cost;
    }

    template <class T>
    inline typename Vertex<T>::ValueType Vertex<T>::get_max_edge_cost()
    {
        T max_cost = std::numeric_limits<T>::lowest();
        for (const auto &edge : edges)
        {
            max_cost = std::max(max_cost, edge->get_cost());
        }
        return max_cost;
    }

    template <class T>
    inline std::string Vertex<T>::to_string() const
    {
        std::ostringstream oss;
        oss << "Vertex " << position << " at " << coordinates;
        return oss.str();
    }

    // Debugging Methods
    template <class T>
    inline void Vertex<T>::print_coordinates() const
    {
        std::cout << coordinates.x() << " " << coordinates.y() << std::endl;
    }

    template <class T>
    inline void Vertex<T>::print_edges() const
    {
        for (const auto &edge : edges)
        {
            edge->get_destination()->print_coordinates();
        }
    }

    template <class T>
    inline void Vertex<T>::print_position() const
    {
        std::cout << position << std::endl;
    }

    template <class T>
    struct Vertex_Compare
    {
        bool operator()(const Vertex<T> *v1, const Vertex<T> *v2) const
        {
            return v1->get_coordinates().x() < v2->get_coordinates().x() ||
                   v1->get_coordinates().y() < v2->get_coordinates().y();
        }
    };
} // namespace graph

#endif // VERTEX_H

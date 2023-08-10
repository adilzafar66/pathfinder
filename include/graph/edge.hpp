#pragma once

#include <iostream>
#include <CGAL/squared_distance_2.h>

namespace graph
{
    template <class T>
    class Vertex;

    template <class T>
    class Edge
    {
    public:
        using VertexPtr = Vertex<T> *;
        using ValueType = T;

        Edge();
        Edge(double cost);
        Edge(VertexPtr src, VertexPtr dest, double cost);
        ~Edge();

        VertexPtr get_source();
        VertexPtr get_destination();
        ValueType get_length();
        double get_cost();
        bool is_connected(VertexPtr v);
        bool is_valid();

        void set_source(VertexPtr src);
        void set_destination(VertexPtr dest);
        void set_cost(double cost);
        void set_source_destination(VertexPtr src, VertexPtr dest);
        void reverse_direction();
        void update_cost(double new_cost);

        bool operator==(const Edge<T> &other);
        bool operator!=(const Edge<T> &other);
        bool operator<(const Edge<T> &other);
        bool operator>(const Edge<T> &other);
        std::string to_string() const;

    private:
        VertexPtr source;
        VertexPtr destination;
        double cost;
    };

    template <class T>
    inline Edge<T>::Edge() : source(nullptr), destination(nullptr), cost(0.0) {}

    template <class T>
    inline Edge<T>::Edge(double cost) : source(nullptr), destination(nullptr), cost(cost) {}

    template <class T>
    inline Edge<T>::Edge(VertexPtr src, VertexPtr dest, double cost) : source(src), destination(dest), cost(cost) {}

    template <class T>
    inline Edge<T>::~Edge() {}

    template <class T>
    inline void Edge<T>::set_source(VertexPtr src)
    {
        source = src;
    }

    template <class T>
    inline void Edge<T>::set_destination(VertexPtr dest)
    {
        destination = dest;
    }

    template <class T>
    inline void Edge<T>::set_cost(double cost)
    {
        this->cost = cost;
    }

    template <class T>
    inline void Edge<T>::set_source_destination(VertexPtr src, VertexPtr dest)
    {
        set_source(src);
        set_destination(dest);
    }

    template <class T>
    inline Vertex<T> *Edge<T>::get_source()
    {
        return source;
    }

    template <class T>
    inline Vertex<T> *Edge<T>::get_destination()
    {
        return destination;
    }

    template <class T>
    inline double Edge<T>::get_cost()
    {
        return cost;
    }

    template <class T>
    inline typename Edge<T>::ValueType Edge<T>::get_length()
    {
        return T(CGAL::sqrt(CGAL::squared_distance(
            source->get_coordinates(),
            destination->get_coordinates())));
    }

    template <class T>
    inline void Edge<T>::reverse_direction()
    {
        std::swap(source, destination);
    }

    template <class T>
    inline void Edge<T>::update_cost(double new_cost)
    {
        cost = new_cost;
    }

    template <class T>
    inline bool Edge<T>::is_valid()
    {
        return source != nullptr && destination != nullptr;
    }

    template <class T>
    inline bool Edge<T>::is_connected(VertexPtr v)
    {
        return source == v || destination == v;
    }

    template <class T>
    inline bool Edge<T>::operator==(const Edge<T> &other)
    {
        return source == other.get_source() && destination == other.get_destination() && cost == other.get_cost();
    }

    template <class T>
    inline bool Edge<T>::operator!=(const Edge<T> &other)
    {
        return !(*this == other);
    }

    template <class T>
    inline bool Edge<T>::operator<(const Edge<T> &other)
    {
        return cost < other.get_cost();
    }

    template <class T>
    inline bool Edge<T>::operator>(const Edge<T> &other)
    {
        return cost > other.get_cost();
    }

    template <class T>
    inline std::string Edge<T>::to_string() const
    {
        std::ostringstream oss;
        oss << "(" << source->get_position() << " -> " << destination->get_position() << ", " << cost << ")";
        return oss.str();
    }
} // namespace graph

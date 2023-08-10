#ifndef READER_H
#define READER_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include "../graph/graph.hpp"

namespace parser
{
    template <class T>
    class GraphFileReader
    {
    public:
        using StartEndInfo = std::tuple<unsigned int, unsigned int>;
        using VertexInfo = std::tuple<unsigned int, T, T>;
        using EdgeInfo = std::tuple<unsigned int, unsigned int, double>;

        GraphFileReader(const std::string &filename);

        StartEndInfo get_start_end();
        std::vector<VertexInfo> get_vertices();
        std::vector<EdgeInfo> get_edges();

    private:
        std::ifstream file;
        StartEndInfo start_end;
        std::vector<VertexInfo> vertices;
        std::vector<EdgeInfo> edges;
        void read_start_end();
        void read_vertices();
        void read_edges();
    };

    template <class T>
    inline GraphFileReader<T>::GraphFileReader(const std::string &filename) : file(filename)
    {
        if (!file)
        {
            throw std::runtime_error("Error opening file: " + filename);
        }
        read_start_end();
        read_vertices();
        read_edges();
    }

    template <class T>
    inline void GraphFileReader<T>::read_start_end()
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                continue; // Skip empty lines and comments
            }
            unsigned int start, end;
            std::stringstream ss(line);
            ss >> start >> end;
            start_end = std::make_tuple(start, end);
            break;
        }
    }

    template <class T>
    inline void GraphFileReader<T>::read_vertices()
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() && !vertices.empty())
            {
                break; // Stop reading for vertices
            }
            if (line.empty() || line[0] == '#')
            {
                continue; // Skip empty lines and comments
            }
            unsigned int vertex_id;
            T x, y;
            std::stringstream ss(line);
            ss >> vertex_id >> x >> y;
            vertices.emplace_back(vertex_id, x, y);
        }
    }

    template <class T>
    inline void GraphFileReader<T>::read_edges()
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#')
            {
                continue; // Skip empty lines and comments
            }
            double cost;
            unsigned int src_vertex, dest_vertex;
            std::stringstream ss(line);
            ss >> src_vertex >> dest_vertex >> cost;
            edges.emplace_back(src_vertex, dest_vertex, cost);
        }
    }

    template <class T>
    inline typename GraphFileReader<T>::StartEndInfo GraphFileReader<T>::get_start_end()
    {
        return start_end;
    }

    template <class T>
    inline std::vector<typename GraphFileReader<T>::VertexInfo> GraphFileReader<T>::get_vertices()
    {
        return vertices;
    }

    template <class T>
    inline std::vector<typename GraphFileReader<T>::EdgeInfo> GraphFileReader<T>::get_edges()
    {
        return edges;
    }
} // namespace parser

#endif // READER_H
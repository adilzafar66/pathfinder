#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <vector>
#include <tuple>
#include <string>

namespace parser
{
    template <class T>
    class GraphFileWriter
    {
    public:
        using ValueType = T;
        using VertexInfo = std::tuple<unsigned int, T, T>;
        using StartEndInfo = std::tuple<unsigned int, unsigned int>;
        using EdgeInfo = std::tuple<unsigned int, unsigned int, double>;

        GraphFileWriter(const std::string &filename);
        void write_start_end(const StartEndInfo &start_end);
        void write_vertices(const std::vector<VertexInfo> &vertices);
        void write_edges(const std::vector<EdgeInfo> &edges, std::string name = "optimal path");
        void write_cost_distance(const double cost, const ValueType distance);

    private:
        std::ofstream file;
    };

    template <class T>
    GraphFileWriter<T>::GraphFileWriter(const std::string &filename) : file(filename)
    {
        if (!file)
        {
            throw std::runtime_error("Error opening file: " + filename);
        }
    }

    template <class T>
    void GraphFileWriter<T>::write_start_end(const StartEndInfo &start_end)
    {
        file << "# Start and end vertices" << std::endl;
        unsigned int start, end;
        std::tie(start, end) = start_end;
        file << start << " " << end << std::endl
             << std::endl;
    }

    template <class T>
    void GraphFileWriter<T>::write_vertices(const std::vector<VertexInfo> &vertices)
    {
        file << "# Vertices" << std::endl;
        for (auto& vertex : vertices) 
        {
            unsigned int pos, x, y;
            std::tie(pos, x, y) = vertex;
            file << pos << " " << x << " " << y << std::endl;
        }
        file << std::endl;
    }

    template <class T>
    void GraphFileWriter<T>::write_edges(const std::vector<EdgeInfo> &edges, std::string name)
    {
        file << "# " << name << " edges" << std::endl;
        for (const auto &edge : edges)
        {
            double cost;
            int src_vertex, dest_vertex;
            std::tie(src_vertex, dest_vertex, cost) = edge;
            file << src_vertex << " " << dest_vertex << " " << cost << std::endl;
        }
        file << std::endl;
    }

    template <class T>
    void GraphFileWriter<T>::write_cost_distance(const double cost, const ValueType distance)
    {
        file << "# Cost and distance" << std::endl;
        file << cost << " " << distance << std::endl
             << std::endl;
    }
} // namespace parser

#endif // WRITER_H
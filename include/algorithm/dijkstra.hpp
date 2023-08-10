#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../graph/graph.hpp"
#include <queue>

using namespace graph;

namespace algorithm
{
    template <class T>
    void compute_dijkstra(Graph<T> &graph, unsigned int start_position, unsigned int end_position)
    {

        using DistPos = std::pair<double, unsigned int>;
        std::unordered_map<unsigned int, double> costs;
        std::unordered_map<unsigned int, unsigned int> previous;
        std::priority_queue<DistPos, std::vector<DistPos>, std::greater<DistPos>> queue;

        for (const auto &entry : graph.get_vertices())
        {
            auto vertex_pos = entry.first;
            costs[vertex_pos] = (vertex_pos == start_position) ? 0 : std::numeric_limits<double>::infinity();
            previous[vertex_pos] = UINT_MAX;
            queue.push(std::make_pair(costs[vertex_pos], vertex_pos));
        }

        // The distance to the start vertex is 0
        costs[start_position] = 0.0;
        queue.push({0.0, start_position});

        while (!queue.empty())
        {
            unsigned int current_vertex_pos = queue.top().second;
            queue.pop();

            // Explore the neighbors of the current vertex
            for (const auto &edge : graph.get_vertex(current_vertex_pos)->get_edges())
            {
                auto neighbor_vertex_pos = edge->get_destination()->get_position();
                auto cost = edge->get_cost();
                if (cost == -1)
                    continue; // Skip untraversable edges

                // Calculate the total distance to the neighbor vertex via the current vertex
                double total_cost = costs[current_vertex_pos] + cost;

                if (total_cost < costs[neighbor_vertex_pos])
                {
                    costs[neighbor_vertex_pos] = total_cost;
                    previous[neighbor_vertex_pos] = current_vertex_pos;
                    queue.push({total_cost, neighbor_vertex_pos});
                }
            }
        }
        // Reconstruct the shortest path from the previous map
        std::vector<unsigned int> path;
        unsigned int current_vertex_pos = end_position;
        while (current_vertex_pos != UINT_MAX && current_vertex_pos != start_position)
        {
            path.push_back(current_vertex_pos);
            current_vertex_pos = previous[current_vertex_pos];
        }
        path.push_back(start_position);
        std::reverse(path.begin(), path.end());
        graph.set_dijkstra_path(path);
    }
} // namespace algorithm

#endif // DIJKSTRA_H

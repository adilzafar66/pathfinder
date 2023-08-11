#ifndef ASTAR_H
#define ASTAR_H

#include <queue>
#include "../graph/graph.hpp"

using namespace graph;

namespace algorithm
{
    std::vector<unsigned int> reconstruct_astar_path(const std::unordered_map<unsigned int, unsigned int> &came_from, unsigned int current_position);

    template <class T>
    void compute_astar(Graph<T> &graph, unsigned int start_position, unsigned int goal_position)
    {
        // Initialize data structures for A* algorithm
        using DistPos = std::pair<double, unsigned int>;
        std::unordered_map<unsigned int, double> g_score;         // Cost from start to each vertex
        std::unordered_map<unsigned int, double> f_score;         // Estimated total cost from start to goal through each vertex
        std::unordered_map<unsigned int, unsigned int> came_from; // Previous vertex in the optimal path
        std::priority_queue<DistPos, std::vector<DistPos>, std::greater<DistPos>> open_set;

        // Initialize scores to infinity for all vertices except the start vertex
        for (const auto &vertex_entry : graph.get_vertices())
        {
            unsigned int vertex_position = vertex_entry.first;
            g_score[vertex_position] = (vertex_position == start_position) ? 0 : std::numeric_limits<double>::infinity();
            f_score[vertex_position] = (vertex_position == start_position) ? graph.get_heuristic(start_position, goal_position) : std::numeric_limits<double>::infinity();
            open_set.push(std::make_pair(f_score[vertex_position], vertex_position));
        }

        // A* algorithm
        while (!open_set.empty())
        {
            unsigned int current_position = open_set.top().second;
            open_set.pop();

            if (current_position == goal_position)
            {
                auto optimal_path = reconstruct_astar_path(came_from, current_position);
                optimal_path.insert(optimal_path.begin(), start_position);
                graph.set_astar_path(optimal_path);
                return;
            }

            const auto current_vertex = graph.get_vertex(current_position); // Use your method to get the vertex

            for (const auto &edge : current_vertex->get_edges())
            {
                unsigned int neighbor_position = edge->get_destination()->get_position();
                double edge_weight = edge->get_cost();

                if (edge_weight == -1)
                    continue; // Skip untraversable edges

                double tentative_g_score = g_score[current_position] + edge_weight;

                if (tentative_g_score < g_score[neighbor_position])
                {
                    came_from[neighbor_position] = current_position;
                    g_score[neighbor_position] = tentative_g_score;
                    f_score[neighbor_position] = g_score[neighbor_position] + graph.get_heuristic(neighbor_position, goal_position);

                    // Update the priority queue with the new f_score
                    open_set.push(std::make_pair(f_score[neighbor_position], neighbor_position));
                }
            }
        }
        // No path found
        graph.set_astar_path(std::vector<unsigned int>());
    }

    std::vector<unsigned int> reconstruct_astar_path(const std::unordered_map<unsigned int, unsigned int> &came_from, unsigned int current_position)
    {
        std::vector<unsigned int> path;
        while (came_from.find(current_position) != came_from.end())
        {
            path.push_back(current_position);
            current_position = came_from.at(current_position);
        }
        if (!path.empty()) // Only reverse the path if it's not empty
            std::reverse(path.begin(), path.end());
        return path;
    }
} // namespace algorithm

#endif // ASTAR_H

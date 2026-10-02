#include "graph.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <limits>

template<typename Vertex, typename Distance>
Vertex find_min_avg_distance_vertex(const Graph<Vertex, Distance>& g) {
    const Distance INF = std::numeric_limits<Distance>::max();
    std::vector<Vertex> verts = g.vertices();
    
    Vertex best = verts[0];
    Distance best_avg = INF;
    
    for (const auto& warehouse : verts) {
        Distance total = Distance{};
        size_t count = 0;
        
        for (const auto& target : verts) {
            if (warehouse == target) continue;
            
            auto path = g.shortest_path(warehouse, target);
            if (!path.empty()) {
                Distance dist = Distance{};
                for (const auto& e : path) {
                    dist = dist + e.distance;
                }
                total = total + dist;
                ++count;
            }
        }
        
        if (count > 0) {
            Distance avg = total / static_cast<Distance>(count);
            if (avg < best_avg) {
                best_avg = avg;
                best = warehouse;
            }
        }
    }
    
    return best;
}

int main() {
    Graph<std::string, double> g;
    
    g.add_vertex("A");
    g.add_vertex("B");
    g.add_vertex("C");
    g.add_vertex("D");
    g.add_vertex("E");
    
    g.add_edge("A", "B", 4.0);
    g.add_edge("A", "C", 2.0);
    g.add_edge("B", "C", 1.0);
    g.add_edge("B", "D", 5.0);
    g.add_edge("C", "D", 8.0);
    g.add_edge("C", "E", 3.0);
    g.add_edge("D", "E", 2.0);
    g.add_edge("E", "D", 6.0);
    
    g.print();
    
    std::cout << "\nBFS walk from A:\n";
    std::vector<std::string> visited;
    auto action = [&visited](const std::string& v) {
        std::cout << v << " ";
        visited.push_back(v);
    };
    g.walk("A", action);
    
    std::cout << "\n\nShortest path from A to D:\n";
    auto path = g.shortest_path("A", "D");
    for (const auto& e : path) {
        std::cout << e.from << " -> " << e.to << " (" << e.distance << ") ";
    }
    
    std::string warehouse = find_min_avg_distance_vertex(g);
    std::cout << "\n\nOptimal warehouse location: " << warehouse << "\n";
    
    return 0;
}
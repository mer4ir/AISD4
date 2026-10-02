#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <utility>

template<typename Vertex, typename Distance = double>
class Graph {
public:
    struct Edge {
        Vertex from;
        Vertex to;
        Distance distance;
        
        Edge();
        Edge(const Vertex& f, const Vertex& t, const Distance& d);
        bool operator==(const Edge& other) const;
    };

    bool has_vertex(const Vertex& v) const;
    bool add_vertex(const Vertex& v);
    bool remove_vertex(const Vertex& v);
    std::vector<Vertex> vertices() const;

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d);
    bool remove_edge(const Vertex& from, const Vertex& to);
    bool remove_edge(const Edge& e);
    bool has_edge(const Vertex& from, const Vertex& to) const;
    bool has_edge(const Edge& e) const;
    
    std::vector<Edge> edges(const Vertex& vertex) const;

    size_t order() const;
    size_t degree(const Vertex& v) const;
    bool is_connected() const;

    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const;
    
    std::vector<Vertex> walk(const Vertex& start_vertex, 
                            std::function<void(const Vertex&)> action) const;
    
    void print() const;

    size_t in_degree(const Vertex& v) const;
    size_t out_degree(const Vertex& v) const;
    bool is_strongly_connected() const;

private:
    std::unordered_map<Vertex, std::vector<Edge>> adj_list_;
    
    std::unordered_set<Vertex> bfs_reverse(const Vertex& start) const;
};

#include "graph.cpp"

#endif
#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "graph.hpp"
#include <queue>
#include <algorithm>
#include <iostream>
#include <map>

template<typename Vertex, typename Distance>
Graph<Vertex, Distance>::Edge::Edge() : from(), to(), distance() {}

template<typename Vertex, typename Distance>
Graph<Vertex, Distance>::Edge::Edge(const Vertex& f, const Vertex& t, const Distance& d)
    : from(f), to(t), distance(d) {}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::Edge::operator==(const Edge& other) const {
    return from == other.from && to == other.to && distance == other.distance;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::has_vertex(const Vertex& v) const {
    return adj_list_.find(v) != adj_list_.end();
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::add_vertex(const Vertex& v) {
    if (has_vertex(v)) return false;
    adj_list_[v] = std::vector<Edge>();
    return true;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::remove_vertex(const Vertex& v) {
    if (!has_vertex(v)) return false;
    
    for (auto& pair : adj_list_) {
        auto& edges = pair.second;
        edges.erase(std::remove_if(edges.begin(), edges.end(),
            [&v](const Edge& e) { return e.to == v; }), edges.end());
    }
    
    adj_list_.erase(v);
    return true;
}

template<typename Vertex, typename Distance>
std::vector<Vertex> Graph<Vertex, Distance>::vertices() const {
    std::vector<Vertex> result;
    for (const auto& pair : adj_list_) {
        result.push_back(pair.first);
    }
    return result;
}

template<typename Vertex, typename Distance>
void Graph<Vertex, Distance>::add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
    if (!has_vertex(from)) add_vertex(from);
    if (!has_vertex(to)) add_vertex(to);
    adj_list_[from].emplace_back(from, to, d);
    adj_list_[to].emplace_back(to, from, d);
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::remove_edge(const Vertex& from, const Vertex& to) {
    if (!has_vertex(from)) return false;
    
    auto& edges = adj_list_[from];
    auto it = std::find_if(edges.begin(), edges.end(),
        [&to](const Edge& e) { return e.to == to; });
    
    if (it != edges.end()) {
        edges.erase(it);
        return true;
    }
    return false;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::remove_edge(const Edge& e) {
    if (!has_vertex(e.from)) return false;
    
    auto& edges = adj_list_[e.from];
    auto it = std::find(edges.begin(), edges.end(), e);
    
    if (it != edges.end()) {
        edges.erase(it);
        return true;
    }
    return false;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::has_edge(const Vertex& from, const Vertex& to) const {
    if (!has_vertex(from)) return false;
    
    const auto& edges = adj_list_.at(from);
    return std::any_of(edges.begin(), edges.end(),
        [&to](const Edge& e) { return e.to == to; });
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::has_edge(const Edge& e) const {
    if (!has_vertex(e.from)) return false;
    
    const auto& edges = adj_list_.at(e.from);
    return std::find(edges.begin(), edges.end(), e) != edges.end();
}

template<typename Vertex, typename Distance>
std::vector<typename Graph<Vertex, Distance>::Edge> 
Graph<Vertex, Distance>::edges(const Vertex& vertex) const {
    if (!has_vertex(vertex)) return std::vector<Edge>();
    return adj_list_.at(vertex);
}

template<typename Vertex, typename Distance>
size_t Graph<Vertex, Distance>::order() const {
    return adj_list_.size();
}

template<typename Vertex, typename Distance>
size_t Graph<Vertex, Distance>::in_degree(const Vertex& v) const {
    if (!has_vertex(v)) return 0;
    
    size_t count = 0;
    for (const auto& pair : adj_list_) {
        for (const auto& edge : pair.second) {
            if (edge.to == v) {
                ++count;
            }
        }
    }
    return count;
}

template<typename Vertex, typename Distance>
size_t Graph<Vertex, Distance>::out_degree(const Vertex& v) const {
    if (!has_vertex(v)) return 0;
    return adj_list_.at(v).size();
}

template<typename Vertex, typename Distance>
size_t Graph<Vertex, Distance>::degree(const Vertex& v) const {
    return out_degree(v) + in_degree(v);
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::is_strongly_connected() const {
    if (order() == 0) return true;
    
    auto start = vertices()[0];
    
    auto reachable = bfs(start);
    if (reachable.size() != order()) return false;
    
    auto reverse_reachable = bfs_reverse(start);
    
    return reverse_reachable.size() == order();
}

template<typename Vertex, typename Distance>
std::unordered_set<Vertex> Graph<Vertex, Distance>::bfs_reverse(const Vertex& start) const {
    std::unordered_set<Vertex> visited;
    std::queue<Vertex> q;
    q.push(start);
    visited.insert(start);
    
    while (!q.empty()) {
        Vertex curr = q.front();
        q.pop();
        
        for (const auto& pair : adj_list_) {
            for (const auto& edge : pair.second) {
                if (edge.to == curr && visited.find(pair.first) == visited.end()) {
                    visited.insert(pair.first);
                    q.push(pair.first);
                }
            }
        }
    }
    return visited;
}

template<typename Vertex, typename Distance>
std::vector<typename Graph<Vertex, Distance>::Edge> 
Graph<Vertex, Distance>::shortest_path(const Vertex& from, const Vertex& to) const {
    const Distance INF = std::numeric_limits<Distance>::max();
    std::unordered_map<Vertex, Distance> dist;
    std::unordered_map<Vertex, std::pair<Vertex, Distance>> prev;
    
    for (const auto& pair : adj_list_) {
        dist[pair.first] = INF;
    }
    
    if (!has_vertex(from) || !has_vertex(to)) return std::vector<Edge>();
    
    dist[from] = Distance{};
    
    for (size_t i = 0; i < order() - 1; ++i) {
        for (const auto& pair : adj_list_) {
            const Vertex& u = pair.first;
            if (dist[u] == INF) continue;
            
            for (const auto& edge : pair.second) {
                if (dist[u] + edge.distance < dist[edge.to]) {
                    dist[edge.to] = dist[u] + edge.distance;
                    prev[edge.to] = std::make_pair(u, edge.distance);
                }
            }
        }
    }
    
    std::vector<Edge> path;
    Vertex curr = to;
    
    while (curr != from) {
        auto it = prev.find(curr);
        if (it == prev.end()) return std::vector<Edge>();
        path.emplace_back(it->second.first, curr, it->second.second);
        curr = it->second.first;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

template<typename Vertex, typename Distance>
std::vector<Vertex> Graph<Vertex, Distance>::walk(const Vertex& start_vertex,
                                                   std::function<void(const Vertex&)> action) const {
    std::vector<Vertex> result;
    
    if (!has_vertex(start_vertex)) return result;
    
    std::unordered_set<Vertex> visited;
    std::queue<Vertex> q;
    
    q.push(start_vertex);
    visited.insert(start_vertex);
    
    while (!q.empty()) {
        Vertex curr = q.front();
        q.pop();
        
        result.push_back(curr);
        if (action) action(curr);
        
        for (const auto& edge : edges(curr)) {
            if (visited.find(edge.to) == visited.end()) {
                visited.insert(edge.to);
                q.push(edge.to);
            }
        }
    }
    
    return result;
}

template<typename Vertex, typename Distance>
void Graph<Vertex, Distance>::print() const {
    std::cout << "Graph with " << order() << " vertices:\n";
    for (const auto& pair : adj_list_) {
        std::cout << pair.first << " -> ";
        for (const auto& edge : pair.second) {
            std::cout << "(" << edge.to << "," << edge.distance << ") ";
        }
        std::cout << "\n";
    }
}

#endif
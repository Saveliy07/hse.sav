#pragma once
#include <map>
#include <set>
#include <vector>
#include <stdexcept>
#include <algorithm>

class GraphVisitor {
public:
    virtual void discoverVertex(int v) {}
    virtual void examineEdge(int u, int v) {}
    virtual void finishVertex(int v) {}
    virtual ~GraphVisitor() = default;
};

class Graph {
public:
    using Vertex = int;

    void addVertex(Vertex v) {
        if (adj.find(v) == adj.end()) adj[v] = std::set<Vertex>();
    }

    void addEdge(Vertex u, Vertex v) {
        addVertex(u); addVertex(v);
        adj[u].insert(v);
        adj[v].insert(u);
    }

    bool hasVertex(Vertex v) const { return adj.count(v); }
    bool hasEdge(Vertex u, Vertex v) const {
        if (!hasVertex(u)) return false;
        return adj.at(u).count(v);
    }

    const std::set<Vertex>& neighbors(Vertex v) const { return adj.at(v); }
    
    std::vector<Vertex> getVertices() const {
        std::vector<Vertex> res;
        for (const auto& [v, _] : adj) res.push_back(v);
        return res;
    }

    size_t vertexCount() const { return adj.size(); }
    size_t edgeCount() const {
        size_t count = 0;
        for (const auto& [v, nbs] : adj) count += nbs.size();
        return count / 2;
    }

    bool isLeaf(Vertex v) const { return hasVertex(v) && adj.at(v).size() == 1; }

    void merge(const Graph& other) {
        for (auto v : other.getVertices()) {
            for (auto u : other.neighbors(v)) addEdge(v, u);
        }
    }

    void dfs(Vertex start, GraphVisitor& visitor, std::set<Vertex>& visited) const {
        visited.insert(start);
        visitor.discoverVertex(start);
        for (auto neighbor : neighbors(start)) {
            visitor.examineEdge(start, neighbor);
            if (visited.find(neighbor) == visited.end()) {
                dfs(neighbor, visitor, visited);
            }
        }
        visitor.finishVertex(start);
    }

private:
    std::map<Vertex, std::set<Vertex>> adj;
};
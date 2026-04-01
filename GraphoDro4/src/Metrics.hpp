#pragma once
#include "Graph.hpp"
#include <queue>
#include <iostream>
#include <random>
#include <cstdint>

class GraphMetrics {
public:
    static double Density(const Graph& g) {
        double n = g.vertexCount();
        if (n < 2) return 0;
        return (2.0 * g.edgeCount()) / (n * (n - 1));
    }

    static int ConnectedComponents(const Graph& g) {
        std::set<Graph::Vertex> visited;
        int count = 0;
        GraphVisitor emptyVisitor;
        for (auto v : g.getVertices()) {
            if (visited.find(v) == visited.end()) {
                g.dfs(v, emptyVisitor, visited);
                count++;
            }
        }
        return count;
    }

    static bool IsBipartite(const Graph& g) {
        std::map<Graph::Vertex, int> color;
        for (auto v : g.getVertices()) {
            if (color.count(v)) continue;
            color[v] = 0;
            std::queue<Graph::Vertex> q;
            q.push(v);
            while (!q.empty()) {
                auto curr = q.front(); q.pop();
                for (auto n : g.neighbors(curr)) {
                    if (!color.count(n)) {
                        color[n] = 1 - color[curr];
                        q.push(n);
                    } else if (color[n] == color[curr]) return false;
                }
            }
        }
        return true;
    }

    static int GreedyColoring(const Graph& g) {
        std::map<Graph::Vertex, int> result;
        int max_color = 0;
        for (auto v : g.getVertices()) {
            std::set<int> used;
            for (auto n : g.neighbors(v)) if (result.count(n)) used.insert(result[n]);
            int clr = 0;
            while (used.count(clr)) clr++;
            result[v] = clr;
            max_color = std::max(max_color, clr);
        }
        return max_color + 1;
    }

    static int Diameter(const Graph& g) {
        int max_d = 0;
        for (auto start : g.getVertices()) {
            std::map<int, int> dist;
            std::queue<int> q;
            dist[start] = 0;
            q.push(start);
            while (!q.empty()) {
                int v = q.front(); q.pop();
                for (int u : g.neighbors(v)) {
                    if (dist.find(u) == dist.end()) {
                        dist[u] = dist[v] + 1;
                        max_d = std::max(max_d, dist[u]);
                        q.push(u);
                    }
                }
            }
        }
        return max_d;
    }

    static double Transitivity(const Graph& g) {
        long long triads = 0, triangles = 0;
        auto vertices = g.getVertices();
        for (int v : vertices) {
            long long d = g.neighbors(v).size();
            triads += d * (d - 1) / 2;
        }
        if (triads == 0) return 0.0;
        for (int u : vertices) {
            for (int v : g.neighbors(u)) {
                if (u < v) {
                    for (int w : g.neighbors(v)) {
                        if (v < w && g.hasEdge(u, w)) triangles++;
                    }
                }
            }
        }
        return (3.0 * triangles) / triads;
    }

    static int CountArticulationPoints(const Graph& g) {
        int timer = 0;
        std::map<int, int> tin, low;
        std::set<int> visited, aps;
        for (auto v : g.getVertices()) {
            if (visited.find(v) == visited.end()) {
                dfsAPs(g, v, -1, timer, visited, tin, low, aps);
            }
        }
        return aps.size();
    }

    static int CountBridgesRandomized(const Graph& g) {
        std::map<int, uint64_t> xor_sum;
        std::set<int> visited;
        std::map<int, int> depth;
        int bridges = 0;
        std::mt19937_64 rng(std::random_device{}());
        for (int root : g.getVertices()) {
            if (visited.find(root) == visited.end()) {
                dfsRandomBridges(g, root, -1, 0, visited, depth, xor_sum, bridges, rng);
            }
        }
        return bridges;
    }

private:
    static void dfsAPs(const Graph& g, int v, int p, int& timer, std::set<int>& visited, 
                       std::map<int, int>& tin, std::map<int, int>& low, std::set<int>& aps) {
        visited.insert(v);
        tin[v] = low[v] = timer++;
        int children = 0;
        for (int to : g.neighbors(v)) {
            if (to == p) continue;
            if (visited.count(to)) {
                low[v] = std::min(low[v], tin[to]);
            } else {
                dfsAPs(g, to, v, timer, visited, tin, low, aps);
                low[v] = std::min(low[v], low[to]);
                if (low[to] >= tin[v] && p != -1) aps.insert(v);
                children++;
            }
        }
        if (p == -1 && children > 1) aps.insert(v);
    }

    static void dfsRandomBridges(const Graph& g, int v, int p, int d, std::set<int>& visited, 
                                 std::map<int, int>& depth, std::map<int, uint64_t>& xor_sum, 
                                 int& bridges, std::mt19937_64& rng) {
        visited.insert(v);
        depth[v] = d;
        xor_sum[v] = 0;
        for (int u : g.neighbors(v)) {
            if (u == p) continue;
            if (visited.count(u)) {
                if (depth[u] < depth[v]) { 
                    uint64_t weight = rng();
                    if (weight == 0) weight = 1; 
                    xor_sum[v] ^= weight;
                    xor_sum[u] ^= weight;
                }
            } else {
                dfsRandomBridges(g, u, v, d + 1, visited, depth, xor_sum, bridges, rng);
                xor_sum[v] ^= xor_sum[u]; 
                if (xor_sum[u] == 0) bridges++;
            }
        }
    }
};
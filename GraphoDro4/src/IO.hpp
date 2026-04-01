#pragma once
#include "Graph.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>

class EdgeListParser {
public:
    static Graph parse(std::istream& in) {
        Graph g; int u, v;
        while (in >> u >> v) g.addEdge(u, v);
        return g;
    }
};

class MatrixParser {
public:
    static Graph parse(std::istream& in) {
        Graph g; int n;
        if (!(in >> n)) return g;
        for (int i=0; i<n; ++i)
            for (int j=0; j<n; ++j) {
                int e; in >> e;
                if (e && i < j) g.addEdge(i, j);
            }
        return g;
    }
};

class DimacsParser {
public:
    static Graph parse(std::istream& in) {
        Graph g; std::string line;
        while (std::getline(in, line)) {
            if (line.empty() || line[0] == 'c' || line[0] == 'p') continue;
            std::stringstream ss(line);
            char type; ss >> type;
            if (type == 'e') { int u, v; ss >> u >> v; g.addEdge(u, v); }
        }
        return g;
    }
};

class GraphVizSerializer {
public:
    enum HighlightMode { NONE, SPANNING_TREE, RANDOM_CYCLE };

    static std::string serialize(const Graph& g, HighlightMode mode = NONE) {
        std::stringstream ss;
        ss << "graph G {\n";
        std::set<std::pair<int, int>> seen, highlightEdges;
        std::set<int> visited;
        
        auto vertices = g.getVertices();
        if (!vertices.empty()) {
            if (mode == SPANNING_TREE) {
                buildRandomTree(g, vertices[0], visited, highlightEdges);
            } else if (mode == RANDOM_CYCLE) {
                std::vector<int> path;
                findRandomCycle(g, vertices[0], -1, path, visited, highlightEdges);
            }
        }

        for (auto u : vertices) {
            ss << "  " << u << ";\n";
            for (auto v : g.neighbors(u)) {
                if (seen.count({v, u}) || seen.count({u, v})) continue;
                seen.insert({u, v});
                ss << "  " << u << " -- " << v;
                if ((highlightEdges.count({u,v}) || highlightEdges.count({v,u}))) {
                    if (mode == SPANNING_TREE) ss << " [color=\"red\", penwidth=2.0]";
                    if (mode == RANDOM_CYCLE) ss << " [color=\"blue\", penwidth=2.0]";
                }
                ss << ";\n";
            }
        }
        ss << "}\n";
        return ss.str();
    }
private:
    static void buildRandomTree(const Graph& g, int v, std::set<int>& vis, std::set<std::pair<int,int>>& edges) {
        vis.insert(v);
        std::vector<int> nbs(g.neighbors(v).begin(), g.neighbors(v).end());
        std::shuffle(nbs.begin(), nbs.end(), std::mt19937{std::random_device{}()});
        for(auto n : nbs) {
            if(vis.find(n) == vis.end()) {
                edges.insert({v, n});
                buildRandomTree(g, n, vis, edges);
            }
        }
    }

    static bool findRandomCycle(const Graph& g, int v, int p, std::vector<int>& path, std::set<int>& vis, std::set<std::pair<int,int>>& cycleEdges) {
        vis.insert(v);
        path.push_back(v);
        std::vector<int> nbs(g.neighbors(v).begin(), g.neighbors(v).end());
        std::shuffle(nbs.begin(), nbs.end(), std::mt19937{std::random_device{}()});
        
        for(int n : nbs) {
            if(n == p) continue;
            if(vis.count(n)) {
                auto it = std::find(path.begin(), path.end(), n);
                if (it != path.end()) {
                    for(auto i = it; i != path.end(); ++i) {
                        int next = (i + 1 == path.end()) ? n : *(i + 1);
                        cycleEdges.insert({*i, next});
                    }
                    return true;
                }
            } else {
                if(findRandomCycle(g, n, v, path, vis, cycleEdges)) return true;
            }
        }
        path.pop_back();
        return false;
    }
};

class Program4YouSerializer {
public:
    static std::string serialize(const Graph& g) {
        std::stringstream ss;
        ss << g.vertexCount() << " " << g.edgeCount() << "\n";
        std::set<std::pair<int, int>> seen;
        for (auto u : g.getVertices()) {
            for (auto v : g.neighbors(u)) {
                if (seen.count({v, u}) || seen.count({u, v})) continue;
                seen.insert({u, v});
                ss << u << " " << v << "\n";
            }
        }
        return ss.str();
    }
};
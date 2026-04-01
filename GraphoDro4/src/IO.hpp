#pragma once
#include "Graph.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <random>    // Для std::mt19937 и std::random_device
#include <algorithm>

// --- PARSERS ---
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

// --- SERIALIZERS ---
class GraphVizSerializer {
public:
    static std::string serialize(const Graph& g, bool highlightTree = false) {
        std::stringstream ss;
        ss << "graph G {\n";
        std::set<std::pair<int, int>> seen, treeEdges;
        
        if (highlightTree) {
            std::set<int> visited;
            auto vertices = g.getVertices();
            if(!vertices.empty()) buildRandomTree(g, vertices[0], visited, treeEdges);
        }

        for (auto u : g.getVertices()) {
            ss << "  " << u << ";\n";
            for (auto v : g.neighbors(u)) {
                if (seen.count({v, u}) || seen.count({u, v})) continue;
                seen.insert({u, v});
                ss << "  " << u << " -- " << v;
                if (highlightTree && (treeEdges.count({u,v}) || treeEdges.count({v,u}))) {
                    ss << " [color=\"red\", penwidth=2.0]";
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
};
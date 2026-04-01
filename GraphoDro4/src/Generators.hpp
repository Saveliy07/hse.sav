#pragma once
#include "Graph.hpp"
#include <random>
#include <numeric>

class GraphGenerator {
public:
    static Graph Complete(int n) {
        Graph g;
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j) g.addEdge(i, j);
        return g;
    }

    static Graph CompleteBipartite(int n, int m) {
        Graph g;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) g.addEdge(i, n + j);
        return g;
    }

    static Graph Star(int n) {
        Graph g;
        for (int i = 1; i < n; ++i) g.addEdge(0, i);
        return g;
    }

    static Graph Cycle(int n) {
        Graph g;
        for (int i = 0; i < n; ++i) g.addEdge(i, (i + 1) % n);
        return g;
    }

    static Graph Path(int n) {
        Graph g;
        for (int i = 0; i < n - 1; ++i) g.addEdge(i, i + 1);
        return g;
    }

    static Graph Wheel(int n) {
        Graph g = Cycle(n - 1);
        for (int i = 0; i < n - 1; ++i) g.addEdge(n - 1, i);
        return g;
    }

    static Graph Random(int n, double p) {
        Graph g;
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (dis(gen) < p) g.addEdge(i, j);
        return g;
    }

    static Graph WithConnectedComponents(int n, int k) {
        Graph g;
        if (k > n || k < 1) return g;
        int compSize = n / k, v = 0;
        for (int i = 0; i < k; ++i) {
            int currentSize = (i == k - 1) ? (n - v) : compSize;
            for (int j = 0; j < currentSize - 1; ++j) g.addEdge(v + j, v + j + 1);
            v += currentSize;
        }
        return g;
    }

    static Graph WithBridges(int n, int b) {
        Graph g;
        for (int i = 0; i < b; ++i) g.addEdge(i, i + 1);
        if (n > b + 1) {
            for (int i = b; i < n - 1; ++i) g.addEdge(i, i + 1);
            if (n - b > 2) g.addEdge(n - 1, b); 
        }
        return g;
    }

    static Graph Cubic(int n) {
        Graph g;
        if (n % 2 != 0 || n < 4) return g; 
        for (int i = 0; i < n; ++i) {
            g.addEdge(i, (i + 1) % n); 
            g.addEdge(i, (i + n / 2) % n); 
        }
        return g;
    }

    static Graph WithArticulationPoints(int n, int k) {
        Graph g;
        if (n < k + 2) return g;
        for (int i = 0; i < k; ++i) g.addEdge(i, i + 1);
        if (n > k + 1) {
            for (int i = k + 1; i < n - 1; ++i) g.addEdge(i, i + 1);
            g.addEdge(n - 1, k + 1);
            g.addEdge(k, k + 1);
            if(n - k > 2) g.addEdge(k, n - 1);
        }
        return g;
    }

    static Graph With2Bridges(int n) {
        Graph g;
        if (n < 6) return Cycle(n); 
        int half = n / 2;
        for(int i=0; i<half-1; ++i) g.addEdge(i, i+1);
        g.addEdge(half-1, 0); 
        for(int i=half; i<n-1; ++i) g.addEdge(i, i+1);
        g.addEdge(n-1, half); 
        g.addEdge(0, half); 
        return g;
    }
};
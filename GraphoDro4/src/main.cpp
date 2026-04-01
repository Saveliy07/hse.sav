#include <iostream>
#include <string>
#include "Graph.hpp"
#include "Generators.hpp"
#include "Metrics.hpp"
#include "IO.hpp"

void printMenu() {
    std::cout << "\n========== GraphoDro4 CLI ==========\n"
              << "1. Generate Complete Graph (Kn)\n"
              << "2. Generate Random Graph (Gnp)\n"
              << "3. Generate Graph with Bridges\n"
              << "4. Calculate All Metrics (8 types)\n"
              << "5. Export to GraphViz (DOT) with Spanning Tree\n"
              << "6. Exit\n"
              << "====================================\n"
              << "Choose an option: ";
}

int main() {
    Graph currentGraph;
    bool hasGraph = false;
    int choice;

    std::cout << "Welcome to GraphoDro4 (CLI Mode)!\n";

    while (true) {
        printMenu();
        // Защита от ввода букв вместо чисел
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            int n; 
            std::cout << "Enter number of vertices (n): "; 
            std::cin >> n;
            currentGraph = GraphGenerator::Complete(n);
            hasGraph = true;
            std::cout << "[OK] Generated Complete Graph K" << n << " with " << currentGraph.edgeCount() << " edges.\n";
        } 
        else if (choice == 2) {
            int n; double p;
            std::cout << "Enter n and p (e.g. 10 0.5): "; 
            std::cin >> n >> p;
            currentGraph = GraphGenerator::Random(n, p);
            hasGraph = true;
            std::cout << "[OK] Generated Random Graph with " << currentGraph.vertexCount() << " vertices and " << currentGraph.edgeCount() << " edges.\n";
        }
        else if (choice == 3) {
            int n, b;
            std::cout << "Enter total vertices (n) and number of bridges (b): ";
            std::cin >> n >> b;
            currentGraph = GraphGenerator::WithBridges(n, b);
            hasGraph = true;
            std::cout << "[OK] Generated Graph with " << currentGraph.vertexCount() << " vertices and " << currentGraph.edgeCount() << " edges.\n";
        }
        else if (choice == 4) {
            if (!hasGraph) { 
                std::cout << "[!] Generate a graph first!\n"; 
                continue; 
            }
            std::cout << "\n--- Graph Metrics ---\n"
                      << "1. Density:                 " << GraphMetrics::Density(currentGraph) << "\n"
                      << "2. Diameter:                " << GraphMetrics::Diameter(currentGraph) << "\n"
                      << "3. Transitivity:            " << GraphMetrics::Transitivity(currentGraph) << "\n"
                      << "4. Connected Components:    " << GraphMetrics::ConnectedComponents(currentGraph) << "\n"
                      << "5. Bridges (Random Alg):    " << GraphMetrics::CountBridgesRandomized(currentGraph) << "\n"
                      << "6. Artic. Points (DFS):     " << GraphMetrics::CountArticulationPoints(currentGraph) << "\n"
                      << "7. Bipartite:               " << (GraphMetrics::IsBipartite(currentGraph) ? "Yes" : "No") << "\n"
                      << "8. Greedy Chromatic Bound:  " << GraphMetrics::GreedyColoring(currentGraph) << "\n";
        } 
        else if (choice == 5) {
            if (!hasGraph) { 
                std::cout << "[!] Generate a graph first!\n"; 
                continue; 
            }
            std::cout << "\nGraphViz output (Copy the text below and paste it into any online GraphViz viewer):\n\n";
            std::cout << GraphVizSerializer::serialize(currentGraph, true) << "\n";
        } 
        else if (choice == 6) {
            std::cout << "Exiting...\n";
            break;
        } 
        else {
            std::cout << "[!] Invalid choice. Try again.\n";
        }
    }
    return 0;
}
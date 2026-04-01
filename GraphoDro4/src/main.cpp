#include <iostream>
#include <string>
#include <fstream>
#include "Graph.hpp"
#include "Generators.hpp"
#include "Metrics.hpp"
#include "IO.hpp"

void printMenu() {
    std::cout << "\n========== GraphoDro4 CLI ==========\n"
              << "1. Generate Complete Graph (Kn)\n"
              << "2. Generate Cubic Graph\n"
              << "3. Generate Graph with Articulation Points\n"
              << "4. Load Graph from Console/File (EdgeList/Matrix/DIMACS)\n"
              << "5. Calculate All Metrics (8 types)\n"
              << "6. Export to GraphViz (DOT) with Spanning Tree\n"
              << "7. Export to GraphViz (DOT) with Random Cycle\n"
              << "8. Export to Progr@m4You (.edges)\n"
              << "9. Exit\n"
              << "====================================\n"
              << "Choose an option: ";
}

int main() {
    Graph currentGraph;
    bool hasGraph = false;
    int choice;

    std::cout << "Welcome to GraphoDro4! (Level 8 Edition)\n";

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear(); std::cin.ignore(10000, '\n'); continue;
        }

        if (choice == 1) {
            int n; std::cout << "Enter n: "; std::cin >> n;
            currentGraph = GraphGenerator::Complete(n);
            hasGraph = true;
            std::cout << "[OK] Generated Complete Graph K" << n << "\n";
        } 
        else if (choice == 2) {
            int n; std::cout << "Enter even n >= 4: "; std::cin >> n;
            currentGraph = GraphGenerator::Cubic(n);
            hasGraph = true;
            std::cout << "[OK] Generated Cubic Graph\n";
        }
        else if (choice == 3) {
            int n, k; std::cout << "Enter total vertices (n) and APs (k): "; std::cin >> n >> k;
            currentGraph = GraphGenerator::WithArticulationPoints(n, k);
            hasGraph = true;
            std::cout << "[OK] Generated Graph with APs\n";
        }
        else if (choice == 4) {
            std::cout << "Select format (1 - EdgeList, 2 - Matrix, 3 - DIMACS):\nChoice: ";
            int fmt; std::cin >> fmt;
            std::cout << "Paste graph data and type 'END' on a new line:\n";
            std::stringstream ss; std::string line;
            while (std::cin >> line && line != "END") ss << line << "\n";
            
            if (fmt == 1) currentGraph = EdgeListParser::parse(ss);
            else if (fmt == 2) currentGraph = MatrixParser::parse(ss);
            else if (fmt == 3) currentGraph = DimacsParser::parse(ss);
            
            hasGraph = true;
            std::cout << "[OK] Loaded Graph with " << currentGraph.edgeCount() << " edges.\n";
        }
        else if (choice == 5 && hasGraph) {
            int aps = 0;
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
        else if (choice == 6 && hasGraph) {
            std::cout << GraphVizSerializer::serialize(currentGraph, GraphVizSerializer::SPANNING_TREE) << "\n";
        }
        else if (choice == 7 && hasGraph) {
            std::cout << GraphVizSerializer::serialize(currentGraph, GraphVizSerializer::RANDOM_CYCLE) << "\n";
        }
        else if (choice == 8 && hasGraph) {
            std::cout << "\n--- Progr@m4You Format ---\n";
            std::cout << Program4YouSerializer::serialize(currentGraph) << "\n";
        }
        else if (choice == 9) break;
        else if (!hasGraph) std::cout << "[!] Generate or load a graph first!\n";
    }
    return 0;
}
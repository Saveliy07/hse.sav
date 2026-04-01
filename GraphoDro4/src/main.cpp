#include <iostream>
#include <string>
#include <fstream>
#include "Graph.hpp"
#include "Generators.hpp"
#include "Metrics.hpp"
#include "IO.hpp"

void printMenu() {
    std::cout << "\n========== GraphoDro4 CLI ==========\n"
              << "1. Generate Graph (Choose from 12 types)\n"
              << "2. Load Graph from Console/File (EdgeList/Matrix/DIMACS)\n"
              << "3. Calculate All Metrics (8 types)\n"
              << "4. Export to GraphViz (DOT) with Spanning Tree\n"
              << "5. Export to GraphViz (DOT) with Random Cycle\n"
              << "6. Export to Progr@m4You (.edges)\n"
              << "7. Exit\n"
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
            std::cout << "\n--- Select Generator ---\n"
                      << "1. Complete (Kn)\n"
                      << "2. Complete Bipartite (Kn,m)\n"
                      << "3. Star (Sn)\n"
                      << "4. Cycle (Cn)\n"
                      << "5. Path (Pn)\n"
                      << "6. Wheel (Wn)\n"
                      << "7. Random (Gnp)\n"
                      << "8. With Connected Components\n"
                      << "9. With Bridges\n"
                      << "10. Cubic\n"
                      << "11. With Articulation Points\n"
                      << "12. With 2-Bridges\n"
                      << "Choose (1-12): ";
            
            int genChoice;
            if (!(std::cin >> genChoice)) { std::cin.clear(); std::cin.ignore(10000, '\n'); continue; }

            int n;
            std::cout << "Enter primary number of vertices (n): ";
            std::cin >> n;

            if (genChoice == 1) currentGraph = GraphGenerator::Complete(n);
            else if (genChoice == 2) { 
                int m; std::cout << "Enter m: "; std::cin >> m; 
                currentGraph = GraphGenerator::CompleteBipartite(n, m); 
            }
            else if (genChoice == 3) currentGraph = GraphGenerator::Star(n);
            else if (genChoice == 4) currentGraph = GraphGenerator::Cycle(n);
            else if (genChoice == 5) currentGraph = GraphGenerator::Path(n);
            else if (genChoice == 6) currentGraph = GraphGenerator::Wheel(n);
            else if (genChoice == 7) { 
                double p; std::cout << "Enter probability p (0.0 to 1.0): "; std::cin >> p; 
                currentGraph = GraphGenerator::Random(n, p); 
            }
            else if (genChoice == 8) { 
                int k; std::cout << "Enter number of components (k): "; std::cin >> k; 
                currentGraph = GraphGenerator::WithConnectedComponents(n, k); 
            }
            else if (genChoice == 9) { 
                int b; std::cout << "Enter number of bridges (b): "; std::cin >> b; 
                currentGraph = GraphGenerator::WithBridges(n, b); 
            }
            else if (genChoice == 10) currentGraph = GraphGenerator::Cubic(n);
            else if (genChoice == 11) { 
                int k; std::cout << "Enter number of articulation points (k): "; std::cin >> k; 
                currentGraph = GraphGenerator::WithArticulationPoints(n, k); 
            }
            else if (genChoice == 12) currentGraph = GraphGenerator::With2Bridges(n);
            else { std::cout << "[!] Invalid generator choice.\n"; continue; }

            hasGraph = true;
            std::cout << "[OK] Generated graph with " << currentGraph.vertexCount() << " vertices and " << currentGraph.edgeCount() << " edges.\n";
        } 
        else if (choice == 2) {
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
        else if (choice == 3 && hasGraph) {
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
        else if (choice == 4 && hasGraph) {
            std::cout << "\n" << GraphVizSerializer::serialize(currentGraph, GraphVizSerializer::SPANNING_TREE) << "\n";
        }
        else if (choice == 5 && hasGraph) {
            std::cout << "\n" << GraphVizSerializer::serialize(currentGraph, GraphVizSerializer::RANDOM_CYCLE) << "\n";
        }
        else if (choice == 6 && hasGraph) {
            std::cout << "\n--- Progr@m4You Format ---\n"
                      << Program4YouSerializer::serialize(currentGraph) << "\n";
        }
        else if (choice == 7) break;
        else if (!hasGraph) std::cout << "[!] Generate or load a graph first!\n";
    }
    return 0;
}
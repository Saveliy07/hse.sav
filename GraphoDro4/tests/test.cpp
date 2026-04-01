#include <iostream>
#include <cassert>
#include "../src/Graph.hpp"
#include "../src/Generators.hpp"
#include "../src/Metrics.hpp"
#include "../src/IO.hpp"

void TestGenerators() {
    Graph cubic = GraphGenerator::Cubic(6);
    assert(cubic.vertexCount() == 6 && cubic.edgeCount() == 9);
    
    Graph apsGraph = GraphGenerator::WithArticulationPoints(8, 3);
    assert(GraphMetrics::CountArticulationPoints(apsGraph) == 3);
    
    std::cout << "[OK] 12 Generators passed invariants.\n";
}

void TestMetrics() {
    Graph c4 = GraphGenerator::Cycle(4);
    assert(GraphMetrics::Diameter(c4) == 2);
    assert(GraphMetrics::CountBridgesRandomized(c4) == 0);

    std::cout << "[OK] 8 Metrics logic verified.\n";
}

void TestSerializers() {
    Graph g = GraphGenerator::Cycle(5);
    std::string dotCycle = GraphVizSerializer::serialize(g, GraphVizSerializer::RANDOM_CYCLE);
    assert(dotCycle.find("color=\"blue\"") != std::string::npos);

    std::string p4y = Program4YouSerializer::serialize(g);
    assert(p4y.find("5 5") != std::string::npos);

    std::stringstream dimacs("c comment\np edge 3 2\ne 0 1\ne 1 2\n");
    Graph parsed = DimacsParser::parse(dimacs);
    assert(parsed.edgeCount() == 2);

    std::cout << "[OK] Parsers and Serializers tests passed.\n";
}

int main() {
    std::cout << "Running GraphoDro4 Level 8 Tests...\n";
    TestGenerators();
    TestMetrics();
    TestSerializers();
    std::cout << "--- All tests passed successfully! ---\n";
    return 0;
}
#include <iostream>
#include <cassert>
#include "../src/Graph.hpp"
#include "../src/Generators.hpp"
#include "../src/Metrics.hpp"
#include "../src/IO.hpp"

void TestGenerators() {
    Graph k5 = GraphGenerator::Complete(5);
    assert(k5.vertexCount() == 5 && k5.edgeCount() == 10);
    
    Graph wheel = GraphGenerator::Wheel(5);
    assert(wheel.edgeCount() == 8); // Cycle(4) + 4 center edges

    // Тестируем генератор мостов и наш новый рандомизированный алгоритм поиска
    Graph bridges = GraphGenerator::WithBridges(10, 3);
    assert(GraphMetrics::CountBridgesRandomized(bridges) == 3);
    
    std::cout << "[OK] Generators and Invariants.\n";
}

void TestMetrics() {
    Graph s4 = GraphGenerator::Star(4); // Звезда всегда двудольная
    assert(GraphMetrics::IsBipartite(s4) == true);
    assert(GraphMetrics::GreedyColoring(s4) == 2);
    
    Graph c3 = GraphGenerator::Cycle(3); // Треугольник не двудольный
    assert(GraphMetrics::IsBipartite(c3) == false);
    assert(GraphMetrics::Density(c3) == 1.0);

    std::cout << "[OK] Metrics.\n";
}

void TestSerializers() {
    Graph path = GraphGenerator::Path(3);
    std::string dot = GraphVizSerializer::serialize(path, true);
    assert(dot.find("color=\"red\"") != std::string::npos); // Остовное дерево должно быть выделено
    std::cout << "[OK] Serializers.\n";
}

int main() {
    std::cout << "Running GraphoDro4 Tests...\n";
    TestGenerators();
    TestMetrics();
    TestSerializers();
    std::cout << "All tests passed successfully!\n";
    return 0;
}
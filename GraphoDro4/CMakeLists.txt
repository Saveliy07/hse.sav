cmake_minimum_required(VERSION 3.10)
project(GraphoDro4 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основное CLI приложение
add_executable(graph_app src/main.cpp)

# Приложение с тестами
add_executable(graph_tests tests/test.cpp)
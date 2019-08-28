cmake_minimum_required(VERSION 3.8)
project(benchmark_sort)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake OPTIONAL)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark)

add_executable(benchmark_sort main.cpp)
target_link_libraries(benchmark_sort benchmark::benchmark_main tbb)

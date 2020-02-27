cmake_minimum_required(VERSION 3.0.0)
project(data-structures VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++2a -fconcepts -Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wnull-dereference -Wduplicated-branches -Wconversion -Wsign-conversion -g)

include_directories(lib)
include_directories(include)

add_library(data-structures INTERFACE)
add_compile_options(-g)
target_include_directories(data-structures INTERFACE include)
target_compile_features(data-structures INTERFACE cxx_std_17)

add_executable(example main.cpp)
target_include_directories(example INTERFACE include)
target_link_libraries(example INTERFACE data-structures)

add_library(catch INTERFACE)
add_executable(tests tests/bst_tests.cpp)
target_include_directories(tests INTERFACE lib)
target_include_directories(tests INTERFACE include)
target_link_libraries(tests INTERFACE catch)
target_link_libraries(tests INTERFACE data-structures)
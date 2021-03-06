cmake_minimum_required(VERSION 3.0.0)
project(data-structures VERSION 0.1.0 LANGUAGES C CXX)

add_compile_options(-std=c++17 -Wall -Wextra -Wpedantic)

enable_testing()
include(CTest)

include_directories(lib)
include_directories(include)

#######################################
## LIBRARY ############################
#######################################

add_library(data-structures INTERFACE)
target_include_directories(data-structures INTERFACE include)

#######################################
## TESTS ##############################
#######################################

# CATCH
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE lib)

add_executable(tests tests/tests_main.cpp tests/bst_tests.cpp tests/rbt_tests.cpp tests/bf_tests.cpp)
target_include_directories(tests INTERFACE include)
target_link_libraries(tests INTERFACE catch)
target_link_libraries(tests INTERFACE data-structures)

add_test(mytests tests)

#######################################
## BENCHMARKS #########################
#######################################
add_executable(st_bench benchmarks/st_bench.cpp)
target_include_directories(st_bench INTERFACE include)
target_link_libraries(st_bench INTERFACE data-structures)

add_executable(rbt_bench benchmarks/rbt_bench.cpp)
target_include_directories(rbt_bench INTERFACE include)
target_link_libraries(rbt_bench INTERFACE data-structures)

add_executable(bf_bench benchmarks/bf_bench.cpp)
target_include_directories(bf_bench INTERFACE include)
target_link_libraries(bf_bench INTERFACE data-structures)
cmake_minimum_required(VERSION 3.0.0)
project(data-structures VERSION 0.1.0)

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
## EXAMPLES ###########################
#######################################

add_executable(example main.cpp)
target_include_directories(example PRIVATE include)
target_link_libraries(example data-structures)

#######################################
## TESTS ##############################
#######################################

# CATCH
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE lib)

add_executable(tests tests/tests_main.cpp tests/bst_tests.cpp tests/rbt_tests.cpp)
target_include_directories(tests INTERFACE include)
target_link_libraries(tests INTERFACE catch)
target_link_libraries(tests INTERFACE data-structures)

add_test(mytests tests)
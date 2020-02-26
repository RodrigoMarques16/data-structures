cmake_minimum_required(VERSION 3.0.0)
project(data-structures VERSION 0.1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++2a -fconcepts)

add_library(data-structures INTERFACE)
add_compile_options(-g)
target_include_directories(data-structures INTERFACE include)
target_compile_features(data-structures INTERFACE cxx_std_17)

add_executable(example main.cpp)
target_include_directories(example INTERFACE include)
target_link_libraries(example INTERFACE data-structures)

add_custom_target(run
    COMMAND example
    DEPENDS example
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
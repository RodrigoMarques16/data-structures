#include "include/bst.hpp"
#include "include/rbt.hpp"
#include "include/st.hpp"
#include "bloom_filter.hpp"

#include <iostream>
#include <algorithm>

int main(/*int argc, char** argv*/) {
    auto bf = bloom_filter<std::string>(100, 0.001);
    bf.add({"hello", "cruel", "world"});
    std::cout << "hello " << bf.test("hello") << '\n';
    std::cout << "cruel " << bf.test("cruel") << '\n';
    std::cout << "world " << bf.test("world") << '\n';
    return 0;
}
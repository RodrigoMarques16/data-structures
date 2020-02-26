#include "include/bst.hpp"
#include <iostream>
int main(int argc, char** argv) {
    BSTree<int> t = {12, 5, 18, 2, 9, 15, 13, 17, 19};
    std::cout << t << '\n';
    t.remove(std::atoi(argv[1]));
    std::cout << t << '\n';
    return 0;
}
#include "include/bst.hpp"
#include "include/rbt.hpp"
#include <iostream>
#include <algorithm>
int main(/*int argc, char** argv*/) {
    RBTree<int> t = {11, 2, 1, 7, 5, 4, 8, 14, 15};
    // std::cout << t <<'\n';
    t.print();
    return 0;
}
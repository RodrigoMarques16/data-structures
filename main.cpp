#include "include/bst.hpp"
#include "include/rbt.hpp"
#include <iostream>
#include <algorithm>
int main(/*int argc, char** argv*/) {
    RBTree<int> t = {12, 5, 18};
    std::cout << t <<'\n';
    t.left_rotate(t.m_root);
    std::cout << t <<'\n';
    return 0;
}
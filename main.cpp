#include "include/bst.hpp"
#include <iostream>
int main() {
    BSTree<int> t = {12, 5, 18, 2, 9,3};
    std::cout << t << '\n';
    std::cout << t.sucessor(9).value_or(-1) << '\n';
    return 0;
}
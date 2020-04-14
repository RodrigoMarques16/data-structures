#include <iostream>

#include "common.hpp"
#include "rbt.hpp"

#define word_count  1000000

int main() {
    auto rb = RBTree<std::string>();
    auto words = read_words(word_count);
    std::cout << "Red-Black Tree"
              << " @ " << word_count << " words\n";

    benchmark("Insertion ", [&](){
        for(int i = 0; i< word_count; i++)
            rb.insert(words[i]);
    });    
    
    return 0;
}
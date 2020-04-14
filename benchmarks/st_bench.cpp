#include <iostream>
#include <algorithm>

#include "common.hpp"
#include "st.hpp"

#define word_count  1000000

int main() {
    auto st = STree<std::string>();
    auto words = read_words(word_count);
    std::cout << words.size() << '\n';
    std::cout << "Splay Tree"
              << " @ " << word_count << " words\n";

    benchmark("Insertion ", [&](){
        for(int i = 0; i< word_count; i++)
            st.insert(words[i]);
    });    
    
    return 0;
}
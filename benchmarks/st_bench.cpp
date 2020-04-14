#include <iostream>

#include "common.hpp"
#include "st.hpp"

#define word_count  1000000

int main() {
    auto st = STree<std::string>();
    auto words = read_words(word_count, "words");
    std::cout << words.size() << '\n';
    std::cout << "Splay Tree"
              << " @ " << word_count << " words\n";

    benchmark("Insertion ", [&](){
        for(int i = 0; i< word_count; i++)
            st.insert(words[i]);
    });

    auto search_words = read_words(word_count, "shuffled_words");

    int count = 0;
    benchmark("Search ", [&](){
        for(int i = 0; i< word_count; i++)
            count += st.contains(search_words[i]);
    });
    
    return count;
}
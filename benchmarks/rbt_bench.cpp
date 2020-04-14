#include <iostream>

#include "common.hpp"
#include "rbt.hpp"

#define word_count  1000000

int main() {
    auto rbt = RBTree<std::string>();
    auto words = read_words(word_count, "words");

    std::cout << "Red Black Tree"
              << " @ " << word_count << " words\n";

    benchmark("Insertion ", [&](){
        for(int i = 0; i< word_count; i++)
            rbt.insert(words[i]);
    });    

    auto search_words = read_words(word_count, "shuffled_words");

    int count = 0;
    benchmark("Search ", [&]() {
        for (int i = 0; i < word_count; i++)
            count += rbt.contains(search_words[i]);
    });

    return count;
}
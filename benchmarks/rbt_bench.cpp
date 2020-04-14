#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "common.hpp"
#include "rbt.hpp"

using namespace std::chrono;

#define word_count  1000000

int main() {
    auto rbt = RBTree<std::string>();
    auto words = read_words(word_count,"./benchmarks/words");

    std::cout << "Red Black Tree"
              << " @ " << word_count << " words\n";

    benchmark("Insertion ", [&](){
        for(int i = 0; i< word_count; i++)
            rbt.insert(words[i]);
    });    

    auto search_words = read_words(word_count, "./benchmarks/shuffled_words");

    benchmark("Search ", [&](){
        for(int i = 0; i< word_count; i++)
            rbt.insert(search_words[i]);
    });

    return 0;
}
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "common.hpp"
#include "st.hpp"

using namespace std::chrono;

#define word_count  1000000

int main() {
    auto st = STree<std::string>();
    auto words = read_words(1000000,"./benchmarks/words");

    std::cout << "Splay Tree"
              << " @ " << word_count << " words\n";

    benchmark("Insertion ", [&](){
        for(int i = 0; i< word_count; i++)
            st.insert(words[i]);
    });    

    return 0;
}
#include <iostream>

#include "bloom_filter.hpp"
#include "common.hpp"

#define bf_set_size 1000000
#define bf_fp_prob  1e-09
#define word_count  1000000

int main() {
    auto bf = bloom_filter<std::string>(bf_set_size, bf_fp_prob);
    auto words = read_words(word_count);

    std::cout << "Bloom filter"
              << " n = " << bf_set_size 
              << " p = " << bf_fp_prob 
              << " k = " << bf.get_hash_func_count()
              << " m = " << bf.get_bit_count()
              << " @ " << word_count << " words\n";

    benchmark("Insertion", [&](){
        for(int i = 0; i < word_count; ++i)
            bf.add(words[i]);
    });

    benchmark("Testing", [&](){
        bool b = true;
        for(int i = 0; i < word_count; ++i)
            b = b && bf.test(words[i]);
    });

    return 0;
}
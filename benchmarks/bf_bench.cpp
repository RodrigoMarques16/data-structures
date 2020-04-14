#include "bloom_filter.hpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std::chrono;

#define bf_set_size 1000000
#define bf_fp_prob  1e-09
#define word_count  1000000

int main() {
    auto bf = bloom_filter<std::string>(bf_set_size, bf_fp_prob);
    std::ifstream fs("words"); 
    std::vector<std::string> words;

    for(int i = 0; i < word_count; ++i){
        char buf[32];
        fs.getline(buf, 32);
        words.emplace_back(buf);
    }

    std::cout << "Bloom filter"
              << " n = " << bf_set_size 
              << " p = " << bf_fp_prob 
              << " k = " << bf.get_hash_func_count()
              << " m = " << bf.get_bit_count()
              << " @ " << word_count << " words\n";

    auto t1 = high_resolution_clock::now();
    
    for(int i = 0; i < word_count; ++i)
        bf.add(words[i]);
    
    auto t2 = high_resolution_clock::now();

    std::cout << "Insertion: " 
          << duration_cast<microseconds>(t2 - t1).count() << "μs, "
          << duration_cast<milliseconds>(t2 - t1).count() << "ms, "
          << duration_cast<seconds>(t2 - t1).count()      << "s\n";

    t1 = high_resolution_clock::now();

    bool b = true;
    for(int i = 0; i < word_count; ++i)
        b = b && bf.test(words[i]);

    t2 = high_resolution_clock::now();

    std::cout << "Testing: " 
              << duration_cast<microseconds>(t2 - t1).count() << "μs, "
              << duration_cast<milliseconds>(t2 - t1).count() << "ms, "
              << duration_cast<seconds>(t2 - t1).count()      << "s\n";

    return 0;
}
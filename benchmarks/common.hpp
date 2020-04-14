#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <functional>
#include <cassert>

std::vector<std::string> read_words(int count = 1000000, const char* path = "words") {
    std::ifstream fs(path);
    std::vector<std::string> words;
    char buf[64];

    assert(fs.is_open());

    while(count--) {
        fs.getline(buf, 64);
        words.emplace_back(buf);
    }

    return words;
}

void benchmark(std::string_view out, std::function<void()> f) {
    using namespace std::chrono;
    
    auto t1 = high_resolution_clock::now();
    f();
    auto t2 = high_resolution_clock::now();
    
    std::cout << out
              << duration_cast<microseconds>(t2 - t1).count() << "μs, "
              << duration_cast<milliseconds>(t2 - t1).count() << "ms, "
              << duration_cast<seconds>(t2 - t1).count()      << "s\n";
}

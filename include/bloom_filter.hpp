#pragma once

#include <memory>
#include <bitset>
#include <functional>

template<
    typename value_type, 
    typename hash_func,
    int bit_count = 50,
    int hash_func_count = 1>
struct bloom_filter {

    // using hash_func = std::function<size_t(value_type)>;

    std::bitset<bit_count> B;
    std::array<hash_func, hash_func_count> hash_funcs = { std::hash<value_type>{} };

    bloom_filter(std::initializer_list<value_type> vals) {
        for (auto& val : vals)
            add(val);
    }

    void add(value_type val) {
        for(auto& h : hash_funcs)
            B.set(h(val));
    }

    bool test(value_type val) {
        for (auto& h : hash_funcs)
            if (B[h(val)] == 0) 
                return false;
        return true;
    }

};
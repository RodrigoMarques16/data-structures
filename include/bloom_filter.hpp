#pragma once

#include <memory>
#include <bitset>
#include <functional>
#include <array>
#include <cmath>

#include <hash/fnv.h>
#include <hash/xxhash.hpp>

template <typename value_type> 
struct Hasher {
    std::pair<uint32_t, uint32_t> operator()(value_type k) const {
        auto a = FNV::fnv1a(k);
        auto b = xxh::xxhash<32>(k);
        return {a, b};
    }
};

template <typename value_type, 
          typename hash_func = Hasher<value_type>>
struct bloom_filter {

    using container = std::vector<bool>;

    bloom_filter(size_t n, double p) {
        using namespace std;

        int m = ceil(n * log(p) / log(1 / pow(2, log(2))));
        int k = round((m / n) * log(2));

        bit_count = m;
        hash_func_count = k;
        B.resize(m);
    }

    void add(const value_type& val) {
        auto [a, b] = hash_values(val);
        for (size_t i = 0; i < hash_func_count; ++i)
            B[nthHash(i, a, b)] = true;
    }

    void add(const std::initializer_list<value_type>& vals) {
        for (auto val : vals)
            add(val);
    }

    bool test(const value_type& val) const {
        auto [a, b] = hash_values(val);
        for (size_t i = 0; i < hash_func_count; ++i)
            if (B[nthHash(i, a, b)] == false)
                return false;
        return true;
    }

    size_t get_bit_count() { return bit_count; }

    size_t get_hash_func_count() { return hash_func_count; }

  private:
    size_t bit_count;
    size_t hash_func_count;
    container B;

    const hash_func hash_values = hash_func{};

    size_t nthHash(int n, size_t a, size_t b) const {
        return (a + n * b) % bit_count;
    }
};
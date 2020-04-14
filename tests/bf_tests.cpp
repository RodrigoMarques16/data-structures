#include <catch.hpp>
#include <bloom_filter.hpp>

TEST_CASE("Bloom filter", "[data-structure]") {
    SECTION("1 in 2") {
        bloom_filter<std::string> bf(10, 0.5);
        bf.add({"hello", "world", "foo", "bar"});
        
        CHECK(bf.get_bit_count() == 15);
        CHECK(bf.get_hash_func_count() == 1);

        CHECK(bf.test("hello") == true);
        CHECK(bf.test("world") == true);
        CHECK(bf.test("foo")   == true);
        CHECK(bf.test("bar")   == true);

        CHECK(bf.test("not")   == true);
        CHECK(bf.test("yes")   == false);
    }

    SECTION("1 in 100") {
        bloom_filter<std::string> bf(100, 0.01);
        bf.add({"hello", "world", "foo", "bar"});
        
        CHECK(bf.get_bit_count() == 959);
        CHECK(bf.get_hash_func_count() == 7);

        CHECK(bf.test("hello") == true);
        CHECK(bf.test("world") == true);
        CHECK(bf.test("foo")   == true);
        CHECK(bf.test("bar")   == true);

        CHECK(bf.test("not")   == false);
        CHECK(bf.test("yes")   == false);
    }

}
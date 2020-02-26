#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <bst.hpp>

TEST_CASE("Vectors", "[math]") {
    SECTION("default construction") {
        BSTree<int> t = {1,2,3};
        t.map([](int x){return x * 2;});
        CHECK(t.size() == 3);
        CHECK(t.contains(2));
        CHECK(t.contains(4));
        CHECK(t.contains(6));
    }
}
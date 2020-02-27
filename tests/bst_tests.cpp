#include <catch2/catch.hpp>
#include <bst.hpp>

TEST_CASE("Binary Search Trees", "[data-structure]") {
    SECTION("transform") {
        BSTree<int> t = {1,2,3};
        t.transform([](int x){return x * 2;});
        CHECK(t.size() == 3);
        CHECK(t.contains(2));
        CHECK(t.contains(4));
        CHECK(t.contains(6));
    }
}
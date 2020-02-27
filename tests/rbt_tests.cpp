#include <catch.hpp>
#include <rbt.hpp>

TEST_CASE("RedBlack Trees", "[data-structure]") {

    RBTree<int> TREE = {7, 4, 11, 3, 6, 9, 18, 2, 14, 19, 12, 17, 22, 20};

    SECTION("construction") {}

    SECTION("comparison") { 
        CHECK(RBTree<int>{} == RBTree<int>{});
        CHECK(TREE == TREE); 
    }

    SECTION("left rotate") {
        RBTree t = {12, 5, 18};
    }
}
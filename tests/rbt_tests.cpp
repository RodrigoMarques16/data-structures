#include <catch.hpp>
#include <rbt.hpp>

TEST_CASE("RedBlack Trees", "[data-structure]") {

    using Tree = RBTree<int>;

    SECTION("construction") {
        Tree e{};
        CHECK(e.empty() == true);
        CHECK(e.size() == 0);

        Tree t = {1, 2, 3};
        CHECK(t.empty() == false);
        CHECK(t.size() == 3);

        Tree t2;
        t2.insert(1);
        t2.insert(2);
        t2.insert(3);
        CHECK(t2.empty() == false);
        CHECK(t2.size() == 3);
        CHECK(t2 == t);
    }

    SECTION("clear") {
        Tree t = {1, 2, 3};
        t.clear();
        CHECK(t == Tree{});
        CHECK(t.empty() == true);
    }

    SECTION("comparison") {
        CHECK(Tree{} == Tree{});
        CHECK(Tree{1, 2, 3} == Tree{1, 2, 3});
        CHECK(Tree{1, 2, 3} != Tree{});
        CHECK(Tree{1, 2, 3} != Tree{3, 2, 1});
    }
}
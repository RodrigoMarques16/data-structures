#include <catch.hpp>
#include <rbt.hpp>

namespace {
    TEST_CASE("RedBlack Trees", "[data-structure]") {
        SECTION("construction") {
        }
        SECTION("left rotate") {
            RBTree t = {12, 5, 18};
            t.left_rotate(t.m_root);
            CHECK(t.m_root->val == 18);
            CHECK(t.m_root->left->val == 12);
            CHECK(t.m_root->left->left->val == 5);
            t.clear();
        }
    }
}
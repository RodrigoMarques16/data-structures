#pragma once

#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>

template <typename N> 
N* parent(N* node) {
    return node ? node->parent : nullptr;
}

template <typename N>
N* grandparent(N* node) {
    return parent(parent(node));
}

template <typename N>
N* sibling(N* node) {
    N* p = parent(node);
    if (p == nullptr)
        return nullptr;
    if (node == p->left.get())
        return p->right.get();
    else
        return p->left.get();
}

template <typename N>
N* uncle(N* node) {
    return sibling(parent(node));
}

template <typename N>
bool is_left_child(N* node) {
    return node->parent 
        && node->parent->left
        && node == node->parent->left.get();
}

template <typename N>
bool is_right_child(N* node) {
    return node->parent 
        && node->parent->right
        && node == node->parent->right.get();
}

template<typename N>
bool is_equal(const N& a, const N& b) {
    if (a == nullptr && b == nullptr)
        return true;
    else if (a == nullptr || b == nullptr)
        return false;
    else return a->val == b->val
             && is_equal(a->left, b->left) 
             && is_equal(b->right, a->right);
}

#pragma once

#include "bst.hpp"
#include "common.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>

template <typename value_type> class STree : private BSTree<value_type> {
    struct Node;

    using raw_ptr = Node *;
    using unique_ptr = std::unique_ptr<Node>;
    
    struct Node {
        value_type val;
        raw_ptr parent = nullptr;
        unique_ptr left = nullptr;
        unique_ptr right = nullptr;
        explicit Node(const value_type &v) : val(v) {}
    };

    unique_ptr m_root;
    size_t m_size = 0;

    unique_ptr &owner(raw_ptr node) {
        auto parent = node->parent;
        if (parent == nullptr)
            return m_root;
        if (parent->left.get() == node)
            return parent->left;
        else
            return parent->right;
    }

    void left_rotate(unique_ptr &x) {
        //    P       Initial state  ->  Final state          P
        //    X       P owns X           P owns Y             Y
        //  A   Y     X owns A and Y     Y owns X and C     X   C
        //    B   C   Y owns B and C     X owns A and B   A   B
        bool is_left = is_left_child(x.get());
        // steal Y from x
        unique_ptr y = std::move(x->right);
        // we own Y
        // X owns A and nullptr
        // Y owns B and C
        // P owns X

        // steal B from Y and give it to X
        x->right = std::move(y->left);
        if (x->right != nullptr)
            x->right->parent = x.get();
        // we own Y,
        // X owns A and B (done)
        // Y owns nullptr and C
        // P owns X

        // set parents
        y->parent = x->parent;
        x->parent = y.get();

        // Y steals X from P
        y->left = std::move(x);
        // we own Y
        // Y owns X and C (done)
        // P owns nullptr
        // ref to x is dangling

        // give Y to P
        if (y->parent == nullptr)
            m_root = std::move(y);
        else if (is_left)
            y->parent->left = std::move(y);
        else
            y->parent->right = std::move(y);
        // we own nothing (done)
        // P owns Y (done)
    }

    void right_rotate(unique_ptr &y) {
        bool is_left = is_left_child(y.get());

        unique_ptr x = std::move(y->left);

        y->left = std::move(x->right);
        if (y->left != nullptr)
            y->left->parent = y.get();

        x->parent = y->parent;
        y->parent = x.get();

        x->right = std::move(y);

        if (x->parent == nullptr)
            m_root = std::move(x);
        else if (is_left)
            x->parent->left = std::move(x);
        else
            x->parent->right = std::move(x);
    }

    bool is_same(const unique_ptr &a, const unique_ptr &b) const {
        if (a == nullptr && b == nullptr)
            return true;
        else if (a == nullptr || b == nullptr)
            return false;
        else
            return a.get() == b.get() && is_same(a->left, b->left) &&
                   is_same(b->right, a->right);
    }

    void rebalance(raw_ptr node) {
        raw_ptr p = parent(node);
        raw_ptr gp = grandparent(node);

        while (gp != nullptr) {
            if (is_left_child(node) == is_left_child(p)) {
                // zig-zig
                if (is_left_child(p)) {
                    right_rotate(owner(gp));
                    right_rotate(owner(p));
                } else {
                    left_rotate(owner(gp));
                    left_rotate(owner(p));
                }
            } else {
                // zig-zag
                if (is_left_child(node)) {
                    right_rotate(owner(p));
                    left_rotate(owner(gp));
                } else {
                    left_rotate(owner(p));
                    right_rotate(owner(gp));
                }
            }
            p = parent(node);
            gp = grandparent(node);
        }

        if (p != nullptr) {
            // zig
            if (is_left_child(node)) {
                right_rotate(owner(p));
            } else {
                left_rotate(owner(p));
            }
        }
    }

  public:
    STree() = default;

    ~STree() {
        release_subtree(std::move(m_root));
    }

    // https://youtu.be/JfmTagWcqoE?t=1122
    void release_subtree(unique_ptr n) {
        while (n->left) {
            auto leaf = std::move(n->left);
            while(leaf->left)
                leaf = std::move(leaf->left);
            leaf->left.release();
            leaf->left = std::move(leaf->right);
        }
        n.release();
    }

    STree(std::initializer_list<value_type> vals) {
        for (auto &val : vals)
            insert(val);
    }

    bool operator==(const STree &other) const {
        return is_same(m_root, other.m_root);
    }

    size_t size() { return m_size; }
    bool empty() { return m_size == 0; }
    void clear() { m_root.release(); }

    void insert(const value_type &val) {
        raw_ptr parent = nullptr;
        auto node = m_root.get();

        while (node != nullptr) {
            parent = node;
            if (val < node->val)
                node = node->left.get();
            else
                node = node->right.get();
        }

        if (parent == nullptr) {
            m_root = std::make_unique<Node>(val);
        } else if (val < parent->val) {
            parent->left = std::make_unique<Node>(val);
            parent->left->parent = parent;
            rebalance(parent->left.get());
        } else {
            parent->right = std::make_unique<Node>(val);
            parent->right->parent = parent;
            rebalance(parent->right.get());
        }

        ++m_size;
    }

    bool contains(const value_type &val) {
        raw_ptr node = m_root.get();
        raw_ptr parent = nullptr;
        while (node != nullptr && val != node->val) {
            parent = node;
            if (val < node->val)
                node = node->left.get();
            else
                node = node->right.get();
        }
        if (node == nullptr) {
            rebalance(parent);
            return false;
        }
        rebalance(node);
        return true;
    }

    friend std::ostream &operator<<(std::ostream &os, unique_ptr &node) {
        if (node == nullptr)
            return os << "empty";
        return os << node->val << " " << node->left << " " << node->right;
    }

    friend std::ostream &operator<<(std::ostream &os, STree &t) {
        return os << "[ " << t.m_root << " ]";
    }

    void print() { print_rec(m_root.get(), "", false); }

    // https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
    void print_rec(raw_ptr node, const std::string &prefix, bool is_left) {
        if (node == nullptr)
            return;

        std::cout << prefix
                  << (is_left ? node->parent->right != nullptr ? "├──" : "└──"
                              : "└──");

        std::cout << std::setw(2) << node->val << '\n';

        print_rec(node->left.get(), prefix + (is_left ? "│   " : "    "), true);
        print_rec(node->right.get(), prefix + (is_left ? "│   " : "    "),
                  false);
    }
};
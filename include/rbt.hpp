#include <memory>
#include <iostream>
#include <optional>
#include "bst.hpp"
#include <cassert>

// 1. a node is red or black 
//
// 2. the root is black
//
// 3. every leaf(nil) is black
//
// 4. if a node is red, its parent is black
//
// 5. for each node, all simple paths from the node to
// descendant leaves contain the same number of black
// nodes

template<typename value_type>
struct RBTree : private BSTree<value_type> {
    struct Node;

    using raw_ptr    = Node*;
    using unique_ptr = std::unique_ptr<Node>;

    enum class rb_color { BLACK, RED };
    struct Node {
        value_type val;
        rb_color color = rb_color::RED;
        raw_ptr parent = nullptr;
        unique_ptr left = nullptr;
        unique_ptr right = nullptr;
        explicit Node(const value_type& v) : val(v) {}
    };

    unique_ptr m_root;
    size_t m_size;

    int black_height(raw_ptr node) {
        return (node->color == rb_color::BLACK) 
             + black_height(node->left)
             + black_height(node->right);
    }

    void left_rotate(unique_ptr& x) {
        //    P       Initial state  ->  Final state          P
        //    X       P owns X           P owns Y             Y
        //  A   Y     X owns A and Y     Y owns X and C     X   C
        //    B   C   Y owns B and C     X owns A and B   A   B

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
        else if (y->left.get() == y->parent->left.get())
            y->parent->left = std::move(y);
        else
            y->parent->right = std::move(y);
        // we own nothing (done)
        // P owns Y (done)
    }

    void right_rotate(unique_ptr& y) {
        unique_ptr x = std::move(y->left);

        y->left = std::move(x->right);
        if (y->left != nullptr)
            y->left->parent = y.get();

        x->parent = y->parent;
        y->parent = x.get();

        x->right = std::move(y);

        if (x->parent == nullptr)
            m_root = std::move(x);
        else if (x->right.get() == x->parent->left.get())
            x->parent->left = std::move(x);
        else
            x->parent->right = std::move(x);
    }

    raw_ptr tree_insert(const value_type& val) {
        raw_ptr parent = nullptr;
        raw_ptr node = m_root.get();
        while (node != nullptr) {
            parent = node;
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        }
        if (parent == nullptr) {
            m_root = std::make_unique<Node>(val);
            return m_root.get();
        } else if (val < parent->val) {
            parent->left = std::make_unique<Node>(val);
            parent->left->parent = parent;
            return parent->left.get();
        } else {
            parent->right = std::make_unique<Node>(val);
            parent->right->parent = parent;
            return parent->right.get();
        }
    }

  public:
    RBTree() = default;

    RBTree(std::initializer_list<value_type> vals) {
        for (auto& val : vals)
            insert(val);
    }

    size_t size() { return m_size; }
    bool empty() {return m_size == 0;}
    void clear() { m_root.release(); }

    void insert(const value_type& val) {
        /*auto node =*/ tree_insert(val);
        // while(node != m_root.get() && node->parent->color == rb_color::RED) {
        //     if (node->parent == node->parent->parent->left.get()){
        //         auto& uncle = node->parent->parent->right;
        //         if (uncle->color == rb_color::RED){
        //             // todo
        //         } else if (node == node->parent->right.get()) {
        //             // todo
        //         }
        //         // todo
        //     } else {

        //     }
        // }
        m_root->color = rb_color::BLACK;
        ++m_size;
    }

    friend std::ostream& operator<<(std::ostream& os, unique_ptr& node){
        if (node == nullptr)
            return os << "empty";
        switch (node->color) {
        case rb_color::RED:
            return os << "\033[31m" << node->val << "\033[0m " << node->left
                      << " " << node->right;
        default:
            return os << node->val << " " << node->left << " " << node->right;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, RBTree& t) {
        return os << "[ " << t.m_root << " ]";
    }

};
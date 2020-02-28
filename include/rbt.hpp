#include "bst.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>

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
class RBTree : private BSTree<value_type> {
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
        explicit Node(const value_type &v) : val(v) {}
    };

    static const unique_ptr NIL;
    unique_ptr m_root;
    size_t m_size;

    unique_ptr &owner(raw_ptr node) {
        auto parent = node->parent;
        if (parent == nullptr)
            return m_root;
        if (parent->left.get() == node)
            return parent->left;
        else
            return parent->right;
    }

    raw_ptr parent(const raw_ptr node) {
        return node ? node->parent : nullptr;
    }

    raw_ptr grandparent(const raw_ptr node) {
        return parent(parent(node));
    }

    raw_ptr sibling(const raw_ptr node) {
        raw_ptr p = parent(node);
        if (p == nullptr)
            return nullptr;
        if (node == p->left.get())
            return p->right.get();
        else
            return p->left.get();
    }

    raw_ptr uncle(const raw_ptr node) {
        return sibling(parent(node));
    }

    bool is_left_child(raw_ptr node) {
        return node->parent 
            && node->parent->left
            && node == node->parent->left.get();
    }

    bool is_right_child(raw_ptr node) {
        return node->parent 
            && node->parent->right
            && node == node->parent->right.get();
    }

    int black_height(const raw_ptr node) const {
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

    bool is_same(const unique_ptr& a, const unique_ptr& b) const {
        if (a == nullptr && b == nullptr)
            return true;
        else if (a == nullptr || b == nullptr)
            return false;
        else return a.get() == b.get() 
                 && is_same(a->left, b->left) 
                 && is_same(b->right, a->right);
    }

    void rebalance(raw_ptr node) {
        raw_ptr p = parent(node);
        raw_ptr y = uncle(node);

        // if (!p) return;
        // if (p->color == rb_color::BLACK)
        //     return;

        if (y && y->color == rb_color::RED) {
            parent(node)->color = rb_color::BLACK;
            uncle(node)->color = rb_color::BLACK;
            grandparent(node)->color = rb_color::RED;
            rebalance(grandparent(node));
        } else if (p && p->color == rb_color::RED /* and y is black*/) {
            if (is_right_child(node) && is_left_child(p)) {
                left_rotate(owner(p));
                node = node->left.get();
            } else if (is_left_child(node) && is_right_child(p)) {
                right_rotate(owner(p));
                node = node->right.get();
            }
            raw_ptr g = grandparent(node);
            if (node == p->left.get())
                right_rotate(owner(g));
            else
                left_rotate(owner(g));
            p->color = rb_color::BLACK;
            g->color = rb_color::RED;
        }
    }

  public:
    RBTree() = default;

    RBTree(std::initializer_list<value_type> vals) {
        for (auto& val : vals)
            insert(val);
    }

    bool operator==(const RBTree& other) const {
        return is_same(m_root, other.m_root);
    }

    size_t size() { return m_size; }
    bool empty() {return m_size == 0;}
    void clear() { m_root.release(); }

    void insert(const value_type& val) {
        raw_ptr parent = nullptr;
        auto node = m_root.get();

        while (node != nullptr) {
            parent = node;
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        }

        if (parent == nullptr){
            m_root = std::make_unique<Node>(val);
            m_root->color = rb_color::BLACK;
        }
        else if (val < parent->val) {
            parent->left = std::make_unique<Node>(val);
            parent->left->parent = parent;
            rebalance(parent->left.get());
        } else {
            parent->right = std::make_unique<Node>(val);
            parent->right->parent = parent;
            rebalance(parent->right.get());
        }

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

    void print() {
        print_rec(m_root.get(), "", false);
    }

    // https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
    void print_rec(raw_ptr node, const std::string& prefix, bool is_left) {
        if (node == nullptr) return;

        std::cout << prefix << (is_left ? "├──" : "└──");
        if (node->color == rb_color::RED)
            std::cout << std::setw(2) << "\033[31m" << node->val << "\033[0m\n";
        else
            std::cout << std::setw(2) << node->val << '\n';
        print_rec(node->left.get(),
                  prefix + (is_left ? "│   " : "    "), 
                  true);
        print_rec(node->right.get(), 
                 prefix + (is_left ? "│   " : "    "), 
                 false);
    }
};
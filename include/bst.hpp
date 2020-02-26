#pragma once

#include <memory>
#include <iostream>
#include <optional>

template<typename value_type>
class BSTree {
    struct Node;

    using pointer = std::unique_ptr<Node>;

    struct Node {
        value_type val;
        Node* parent = nullptr;
        pointer left = nullptr;
        pointer right = nullptr;
        explicit Node(const value_type& v) : val(v) {}
        Node(Node&&) = default;
    };

    pointer m_root;
    size_t m_size;

    Node* find(const value_type& val) const {
        auto node = m_root.get();
        while (node != nullptr && val != node->val)
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        return node;
    }

    pointer& owner(Node* node) {
        auto parent = node->parent;
        if (parent == nullptr)
            return m_root;
        if (parent->left.get() == node)
            return parent->left;
        else return parent->right;
    }

    Node* find_minimum(const pointer& ref) {
        auto node = ref.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node;
    }

    Node* find_maximum(const pointer& ref) {
        auto node = ref.get();
        while (node->right != nullptr)
            node = node->right.get();
        return node;
    }

    Node const* find_minimum(const pointer& ref) const {
        auto node = ref.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node;
    }

    void transform(pointer& node, auto&& f) {
        if (node == nullptr) return;
        transform(node->left, f);
        node->val = f(node->val);
        transform(node->right, f);
    }

  public:
    BSTree() = default;

    BSTree(std::initializer_list<value_type> vals) : m_size(vals.size()) {
        for (auto& val : vals)
            insert(val);
    }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    void clear() { m_root.release(); }

    void insert(const value_type& val) {
        Node* parent = nullptr;
        auto node = m_root.get();
        while (node != nullptr) {
            parent = node;
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        }
        if (parent == nullptr)
            m_root = std::make_unique<Node>(val);
        else if (val < parent->val) {
            parent->left = std::make_unique<Node>(val);
            parent->left->parent = parent;
        } else {
            parent->right = std::make_unique<Node>(val);
            parent->right->parent = parent;
        }
    }

    void remove(const value_type& val) {
        auto node = find(val);
        if (node == nullptr)
            return;
        if (node->left == nullptr) {
            auto& o = owner(node);
            o = std::move(node->right);
        } else if (node->right == nullptr) {
            auto& o = owner(node);
            o = std::move(node->left);
        } else {
            auto succ = find_minimum(node->right);
            node->val = succ->val;
            // succ guaranteed not to be null and to have only a right child
            owner(succ).reset(succ->right.release());
        }
    }

    bool contains(const value_type& val) const {
        auto node = m_root.get();
        while (node != nullptr && val != node->val)
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        return node != nullptr;
    }

    value_type minimum() const {
        auto node = m_root.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node->val;
    }

    value_type maximum() const {
        auto node = m_root.get();
        while (node->right != nullptr)
            node = node->right.get();
        return node->val;
    }

    std::optional<value_type> successor(const value_type& val) const {
        auto node = find(val);
        if (node->right != nullptr) {
            auto succ = find_minimum(node->right);
            if (succ == nullptr)
                return std::nullopt;
            return succ->val;
        }
        auto succ = node->parent;
        while (succ != nullptr and node == succ->right.get()) {
            node = succ;
            succ = succ->parent;
        }
        if (succ == nullptr)
            return std::nullopt;
        return succ->val;
    }

    void transform(auto&& f) { transform(m_root, f); }

    friend std::ostream& operator<<(std::ostream& os, pointer& node){
        if (node == nullptr)
            return os << "empty";
        return os << node->val << " " << node->left << " " << node->right;
    }

    friend std::ostream& operator<<(std::ostream& os, BSTree& t) {
        return os << "[ " << t.m_root << " ]";
    }
};


#pragma once

#include <memory>
#include <iostream>
#include <optional>

template<typename value_type>
class BSTree {
    struct Node;

    using raw_ptr    = Node*;
    using unique_ptr = std::unique_ptr<Node>;

    struct Node {
        value_type val;
        raw_ptr parent = nullptr;
        unique_ptr left = nullptr;
        unique_ptr right = nullptr;
        explicit Node(const value_type& v) : val(v) {}
    };

    unique_ptr m_root = nullptr;
    size_t m_size = 0;

    raw_ptr find(const value_type& val) const {
        auto node = m_root.get();
        while (node != nullptr && val != node->val)
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        return node;
    }

    unique_ptr& owner(raw_ptr node) {
        auto parent = node->parent;
        if (parent == nullptr)
            return m_root;
        if (parent->left.get() == node)
            return parent->left;
        else return parent->right;
    }

    raw_ptr find_minimum(const unique_ptr& ref) {
        auto node = ref.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node;
    }

    raw_ptr find_maximum(const unique_ptr& ref) {
        auto node = ref.get();
        while (node->right != nullptr)
            node = node->right.get();
        return node;
    }

    Node const* find_minimum(const unique_ptr& ref) const {
        auto node = ref.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node;
    }

    void transform(unique_ptr& node, auto&& f) {
        if (node == nullptr) return;
        transform(node->left, f);
        node->val = f(node->val);
        transform(node->right, f);
    }

  public:
    BSTree() = default;

    BSTree(std::initializer_list<value_type> vals) {
        for (auto& val : vals)
            insert(val);
    }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    void clear() { m_root.release(); }

    void insert(const value_type& val) {
        raw_ptr parent = nullptr;
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
        ++m_size;
    }

    void remove(const value_type& val) {
        unique_ptr node = find(val);
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
        --m_size;
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

    friend std::ostream& operator<<(std::ostream& os, unique_ptr& node){
        if (node == nullptr)
            return os << "empty";
        return os << node->val << " " << node->left << " " << node->right;
    }

    friend std::ostream& operator<<(std::ostream& os, BSTree& t) {
        return os << "[ " << t.m_root << " ]";
    }
};


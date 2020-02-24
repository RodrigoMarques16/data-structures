#include <memory>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <queue>

template<typename ValueType>
class BSTree {
    struct Node;
    using PtrType = std::unique_ptr<Node>;
    
    struct Node {
        ValueType val;
        Node* parent = nullptr;
        PtrType left = nullptr;
        PtrType right = nullptr;
        explicit Node(const ValueType& v) : val(v) {}
        Node(Node&&) = default;
    };

    PtrType m_root;
    size_t m_size;

    void transplant(Node* old_node, PtrType& new_node) {
        auto old_parent = old_node->parent;
        if (old_node->parent == nullptr)
            m_root = std::move(new_node);
        else if (old_node == old_node->parent->left.get()) {
            old_node->parent->left = std::move(new_node);
            if (old_node->parent->left != nullptr)
                old_node->parent->left->parent = old_parent;
        } else {
            old_node->parent->right = std::move(new_node);
            if (old_node->parent->right != nullptr)
                old_node->parent->right->parent = old_parent;
        }
    }

    Node* find(const ValueType& val) const {
        auto node = m_root.get();
        while (node != nullptr && val != node->val)
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        return node;
    }

    PtrType& owner(Node* node) {
        auto parent = node->parent;
        if (parent == nullptr)
            return m_root;
        if (parent->left.get() == node)
            return parent->left;
        else return parent->right;
    }

    Node* find_minimum(const PtrType& ref) const {
        auto node = ref.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node;
    }

public:

    BSTree() = default;

    BSTree(std::initializer_list<ValueType> vals) : m_size(vals.size()) {
        for(auto& val : vals)
            insert(val);
    }

    size_t size() const { return m_size; }
    
    void insert(const ValueType& val) {
        Node* parent = nullptr;
        auto node = m_root.get();
        while(node != nullptr){
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

    void remove(const ValueType& val) {
        auto node = find(val);
        if (node == nullptr)
            return;
        if (node->left == nullptr) {
            transplant(node, node->right);
        } else if (node->right == nullptr) {
            transplant(node, node->left);
        } else {
            auto min_node = find_minimum(node->right);
            if (min_node->parent != node) {
                min_node->right = std::move(node->right);
                min_node->right->parent = min_node;
                transplant(min_node, min_node->right);
            }
            min_node->left = std::move(node->left);
            min_node->left->parent = min_node;
            transplant(node, owner(min_node));
        }
    }

    bool contains(const ValueType& val) const {
        auto node = m_root.get();
        while (node != nullptr && val != node->val)
            if (val < node->val) node = node->left.get();
            else                 node = node->right.get();
        return node != nullptr;
    }

    ValueType minimum() const {
        auto node = m_root.get();
        while (node->left != nullptr)
            node = node->left.get();
        return node->val;
    }

    ValueType maximum() const {
        auto node = m_root.get();
        while (node->right != nullptr)
            node = node->right.get();
        return node->val;
    }

    std::optional<ValueType> sucessor(const ValueType& val) const {
        auto node = find(val);
        if (node->right != nullptr) {
            auto min_node = find_minimum(node->right);
            if (min_node == nullptr)
                return std::nullopt;
            return std::make_optional(min_node->val);
        }
        auto succ = node->parent;
        while (succ != nullptr and node == succ->right.get()) {
            node = succ;
            succ = succ->parent;
        }
        if (succ == nullptr)
            return std::nullopt;
        return std::make_optional(succ->val);
    }

    friend std::ostream& operator<<(std::ostream& os, PtrType& node){
        if (node == nullptr)
            return os << "empty";
        return os << node->val << " " << node->left << " " << node->right;
    }

    friend std::ostream& operator<<(std::ostream& os, BSTree& t) {
        return os << "[ " << t.m_root << " ]";
    }

    int depth() {
        auto node = m_root.get();
        int depth = 0;
        while (node != nullptr) {
            node = node->left != nullptr ? node->left.get() : node->right.get();
            ++depth;
        }
        return depth;
    }
};


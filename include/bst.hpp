#include <memory>
#include <algorithm>
#include <iostream>
#include <cassert>

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

    void map(PtrType& node, auto&& f) {
        if (node == nullptr) return;
        map(node->left, f);
        node->val = f(node->val);
        map(node->right, f);
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
        if (node->left == nullptr)
            owner(node).reset(node->right.release());
        else if (node->right == nullptr)
            owner(node).reset(node->left.release());
        else {
            auto succ = find_minimum(node->right);
            auto min_val = succ->val;
            // guaranteed to have only a right child
            owner(succ).reset(succ->right.release());
            node->val = min_val;
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

    std::optional<ValueType> successor(const ValueType& val) const {
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

    void map(auto&& f) {
        map(m_root, f);
    }

    friend std::ostream& operator<<(std::ostream& os, PtrType& node){
        if (node == nullptr)
            return os << "empty";
        return os << node->val << " " << node->left << " " << node->right;
    }

    friend std::ostream& operator<<(std::ostream& os, BSTree& t) {
        return os << "[ " << t.m_root << " ]";
    }
};


#pragma once
#include <memory>
#include <functional>
#include <optional>
#include <algorithm>

template<typename T, typename Key = T, typename KeyFn = std::function<Key(const T&)>>
class AVLTree {
    struct Node {
        T val;
        int h{1};
        std::unique_ptr<Node> left, right;
        explicit Node(const T& v): val(v) {}
    };
    std::unique_ptr<Node> root;
    KeyFn key_of;

    static int height(const std::unique_ptr<Node>& n){ return n? n->h : 0; }
    static int bf(const std::unique_ptr<Node>& n){ return n? height(n->left)-height(n->right):0; }
    static void upd(std::unique_ptr<Node>& n){
        if (!n) return;
        n->h = 1 + std::max(height(n->left), height(n->right));
    }
    static std::unique_ptr<Node> rotate_right(std::unique_ptr<Node> y){
        auto x = std::move(y->left);
        auto T2 = std::move(x->right);
        x->right = std::move(y);
        x->right->left = std::move(T2);
        upd(x->right); upd(x);
        return x;
    }
    static std::unique_ptr<Node> rotate_left(std::unique_ptr<Node> x){
        auto y = std::move(x->right);
        auto T2 = std::move(y->left);
        y->left = std::move(x);
        y->left->right = std::move(T2);
        upd(y->left); upd(y);
        return y;
    }

    std::unique_ptr<Node> insert(std::unique_ptr<Node> n, const T& v){
        if(!n) return std::make_unique<Node>(v);
        if (key_of(v) < key_of(n->val)) n->left  = insert(std::move(n->left), v);
        else if (key_of(n->val) < key_of(v)) n->right = insert(std::move(n->right), v);
        else { n->val = v; return n; }

        upd(n);
        int balance = bf(n);

        if (balance > 1 && key_of(v) < key_of(n->left->val)) return rotate_right(std::move(n));                
        if (balance < -1 && key_of(n->right->val) < key_of(v)) return rotate_left(std::move(n));               
        if (balance > 1 && key_of(n->left->val) < key_of(v)) { n->left = rotate_left(std::move(n->left)); return rotate_right(std::move(n)); }
        if (balance < -1 && key_of(v) < key_of(n->right->val)) { n->right = rotate_right(std::move(n->right)); return rotate_left(std::move(n)); }
        return n;
    }

    static Node* min_node(Node* n){ while(n && n->left) n = n->left.get(); return n; }

    std::unique_ptr<Node> erase(std::unique_ptr<Node> n, const Key& k){
        if(!n) return nullptr;
        if (k < key_of(n->val)) n->left = erase(std::move(n->left), k);
        else if (key_of(n->val) < k) n->right = erase(std::move(n->right), k);
        else {
            if(!n->left || !n->right) {
                return std::move(n->left ? n->left : n->right);
            } else {
                Node* succ = min_node(n->right.get());
                n->val = succ->val;
                n->right = erase(std::move(n->right), key_of(succ->val));
            }
        }

        upd(n);
        int balance = bf(n);

        if (balance > 1 && bf(n->left) >= 0) return rotate_right(std::move(n));
        if (balance > 1 && bf(n->left) < 0)  { n->left = rotate_left(std::move(n->left)); return rotate_right(std::move(n)); }
        if (balance < -1 && bf(n->right) <=0) return rotate_left(std::move(n));
        if (balance < -1 && bf(n->right) > 0) { n->right = rotate_right(std::move(n->right)); return rotate_left(std::move(n)); }
        return n;
    }

    const T* find(const Node* n, const Key& k) const {
        if(!n) return nullptr;
        if (k < key_of(n->val)) return find(n->left.get(), k);
        if (key_of(n->val) < k) return find(n->right.get(), k);
        return &n->val;
    }

public:
    explicit AVLTree(KeyFn key_fn): key_of(std::move(key_fn)) {}
    void insert(const T& v){ root = insert(std::move(root), v); }
    void erase(const Key& k){ root = erase(std::move(root), k); }
    std::optional<T> get(const Key& k) const { if (auto p = find(root.get(), k)) return *p; return std::nullopt; }

    template<typename Fn>
    void inorder(Fn fn) const { inorder_impl(root.get(), fn); }

private:
    template<typename Fn>
    static void inorder_impl(const Node* n, Fn& fn){
        if(!n) return;
        inorder_impl(n->left.get(), fn);
        fn(n->val);
        inorder_impl(n->right.get(), fn);
    }
};

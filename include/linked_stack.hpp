#pragma once
#include <stdexcept>
#include <cstddef>

template<typename T>
class LinkedStack {
    struct Node { T v; Node* next; Node(const T& x, Node* n): v(x), next(n) {} };
    Node* top_{nullptr}; std::size_t n{0};
public:
    ~LinkedStack(){ while(!empty()) pop(); }
    bool empty() const { return n==0; }
    std::size_t size() const { return n; }
    void push(const T& x){ top_ = new Node(x, top_); ++n; }
    T& top(){ if(empty()) throw std::runtime_error("stack empty"); return top_->v; }
    void pop(){ if(empty()) throw std::runtime_error("stack empty"); auto* t=top_; top_=top_->next; delete t; --n; }
};

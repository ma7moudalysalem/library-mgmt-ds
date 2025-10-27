#pragma once
#include <stdexcept>
#include <cstddef>

template<typename T>
class LinkedQueue {
    struct Node { T v; Node* next; Node(const T& x): v(x), next(nullptr) {} };
    Node* head{nullptr}; Node* tail{nullptr}; std::size_t n{0};
    public:
    ~LinkedQueue(){ while(!empty()) pop(); }
    bool empty() const { return n==0; }
    std::size_t size() const { return n; }
    void push(const T& x){ auto* nd = new Node(x); if(tail) tail->next = nd; else head = nd; tail = nd; ++n; }
    T& front(){ if(empty()) throw std::runtime_error("queue empty"); return head->v; }
    void pop(){ if(empty()) throw std::runtime_error("queue empty"); auto* t=head; head=head->next; if(!head) tail=nullptr; delete t; --n; }
};

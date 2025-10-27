#pragma once
#include "book.hpp"
#include "avl_tree.hpp"
#include "linked_queue.hpp"
#include "linked_stack.hpp"
#include "binary_search.hpp"
#include "actions.hpp"
#include <vector>
#include <algorithm>
#include <optional>
#include <string>

struct Request { std::string userId; std::string isbn; };

class Library {
    AVLTree<Book, std::string, std::function<std::string(const Book&)>> catalog;
    LinkedQueue<Request> waitlist;
    LinkedStack<Action> undo;
    std::vector<Book> linear_store;
    std::vector<BookRef> sorted_by_isbn;

    void rebuild_sorted(){
        sorted_by_isbn.clear();
        sorted_by_isbn.reserve(linear_store.size());
        for (int i = 0; i < (int)linear_store.size(); ++i)
            sorted_by_isbn.push_back({linear_store[i].isbn, i});
        std::sort(sorted_by_isbn.begin(), sorted_by_isbn.end(),
                  [](const BookRef& a, const BookRef& b){ return a.isbn < b.isbn; });
    }

public:
    Library(): catalog([](const Book& b){ return b.isbn; }) {}

    bool add_book(const Book& b){
        if (catalog.get(b.isbn).has_value()) return false;
        catalog.insert(b);
        linear_store.push_back(b);
        rebuild_sorted();
        return true;
    }

    bool remove_book(const std::string& isbn){
        if (!catalog.get(isbn).has_value()) return false;
        catalog.erase(isbn);
        auto it = std::find_if(linear_store.begin(), linear_store.end(), [&](auto& x){ return x.isbn==isbn; });
        if (it != linear_store.end()) linear_store.erase(it);
        rebuild_sorted();
        return true;
    }

    std::optional<Book> find_by_isbn_tree(const std::string& isbn) const {
        return catalog.get(isbn);
    }

    std::optional<Book> find_by_isbn_binary(const std::string& isbn) const {
        if (auto idx = binary_search_isbn(sorted_by_isbn, isbn)) return linear_store[*idx];
        return std::nullopt;
    }

    bool checkout(const std::string& user, const std::string& isbn){
        auto b = catalog.get(isbn);
        if(!b.has_value()) return false;
        if (b->copies_available > 0){
            Book updated = *b; updated.copies_available -= 1;
            catalog.insert(updated);
            if (auto idx = binary_search_isbn(sorted_by_isbn, isbn))
                linear_store[*idx].copies_available = updated.copies_available;
            undo.push({ActionType::Checkout, isbn, -1});
            return true;
        } else {
            waitlist.push({user, isbn});
            return false;
        }
    }

    bool give_back(const std::string& isbn){
        auto b = catalog.get(isbn);
        if(!b.has_value()) return false;
        Book updated = *b; updated.copies_available += 1;
        catalog.insert(updated);
        if (auto idx = binary_search_isbn(sorted_by_isbn, isbn))
            linear_store[*idx].copies_available = updated.copies_available;
        undo.push({ActionType::Return, isbn, +1});
        return true;
    }

    bool undo_last(){
        if (undo.empty()) return false;
        auto act = undo.top(); undo.pop();
        auto b = catalog.get(act.isbn);
        if(!b.has_value()) return false;
        Book updated = *b; updated.copies_available -= act.delta; // reverse
        catalog.insert(updated);
        if (auto idx = binary_search_isbn(sorted_by_isbn, act.isbn))
            linear_store[*idx].copies_available = updated.copies_available;
        return true;
    }

    std::optional<Request> next_request(){
        if (waitlist.empty()) return std::nullopt;
        return waitlist.front();
    }
    void pop_request(){ if(!waitlist.empty()) waitlist.pop(); }

    template<typename Fn>
    void list_catalog_inorder(Fn fn) const { catalog.inorder(fn); }
};

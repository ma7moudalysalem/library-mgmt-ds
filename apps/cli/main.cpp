#include "library.hpp"
#include <iostream>

int main(){
    Library lib;

    lib.add_book({"978-0131103627","The C Programming Language","Kernighan & Ritchie",1988,5,5});
    lib.add_book({"978-0321563842","Effective C++","Scott Meyers",2005,3,3});
    lib.add_book({"978-0262033848","Introduction to Algorithms","CLRS",2009,4,4});

    std::cout << "AVL inorder (by ISBN):\n";
    lib.list_catalog_inorder([](const Book& b){
        std::cout << "  " << b.isbn << " | " << b.title << " | avail=" << b.copies_available << "\n";
    });

    std::cout << "\nCheckout CLRS...\n";
    lib.checkout("user1","978-0262033848");

    if (auto b = lib.find_by_isbn_binary("978-0262033848"))
        std::cout << "Binary search found: " << b->title << " avail=" << b->copies_available << "\n";

    std::cout << "Undo last action...\n";
    lib.undo_last();

    if (auto req = lib.next_request())
        std::cout << "Next waitlist request: " << req->userId << " wants " << req->isbn << "\n";
    else
        std::cout << "No pending waitlist requests.\n";

    return 0;
}

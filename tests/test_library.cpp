#include "library.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Library add/find/remove", "[library]") {
    Library lib;
    REQUIRE(lib.add_book({"1","A","auth",2000,2,2}));
    REQUIRE_FALSE(lib.add_book({"1","A","auth",2000,2,2})); // duplicate
    auto b = lib.find_by_isbn_tree("1");
    REQUIRE(b.has_value());
    REQUIRE(b->copies_available == 2);
    REQUIRE(lib.remove_book("1"));
    REQUIRE_FALSE(lib.find_by_isbn_tree("1").has_value());
}

TEST_CASE("Checkout / return / undo / waitlist", "[library]") {
    Library lib;
    lib.add_book({"X","BookX","auth",2000,1,1});

    // checkout succeeds
    REQUIRE(lib.checkout("u1","X") == true);

    // book now unavailable; checkout goes to waitlist
    REQUIRE(lib.checkout("u2","X") == false);
    auto r = lib.next_request();
    REQUIRE(r.has_value());
    REQUIRE(r->userId == "u2");

    // give back increases availability and can be undone
    REQUIRE(lib.give_back("X") == true);
    REQUIRE(lib.undo_last() == true); // undo return => back to unavailable again
}

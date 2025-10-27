#include "binary_search.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>

TEST_CASE("binary_search_isbn finds and misses", "[binary]") {
    std::vector<BookRef> sorted = {
        {"111", 0}, {"222", 1}, {"333", 2}, {"444", 3}
    };
    auto f1 = binary_search_isbn(sorted, "333");
    REQUIRE(f1.has_value());
    REQUIRE(*f1 == 2);

    auto f2 = binary_search_isbn(sorted, "777");
    REQUIRE_FALSE(f2.has_value());
}

#include "avl_tree.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <string>

TEST_CASE("AVL insert/find/inorder sorted", "[avl]") {
    AVLTree<int,int,std::function<int(const int&)>> t([](const int& x){ return x; });
    std::vector<int> vals{5,2,8,1,3,7,9,4,6};
    for (int v : vals) t.insert(v);

    // Find existing
    REQUIRE(t.get(7).has_value());
    REQUIRE(t.get(10).has_value() == false);

    // Inorder is sorted
    std::vector<int> got;
    t.inorder([&](const int& x){ got.push_back(x); });
    REQUIRE(std::is_sorted(got.begin(), got.end()));

    // Erase leaf / one-child / two-children
    t.erase(1);  // leaf
    t.erase(8);  // two-children case after rotations
    REQUIRE_FALSE(t.get(1).has_value());
    REQUIRE_FALSE(t.get(8).has_value());
}

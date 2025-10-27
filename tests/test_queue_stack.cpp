#include "linked_queue.hpp"
#include "linked_stack.hpp"
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <string>

TEST_CASE("LinkedQueue FIFO", "[queue]") {
    LinkedQueue<std::string> q;
    q.push("A"); q.push("B"); q.push("C");
    REQUIRE(q.size() == 3);
    REQUIRE(q.front() == "A");
    q.pop();
    REQUIRE(q.front() == "B");
    q.pop(); q.pop();
    REQUIRE(q.empty());
    REQUIRE_THROWS_AS(q.pop(), std::runtime_error);
}

TEST_CASE("LinkedStack LIFO", "[stack]") {
    LinkedStack<int> st;
    st.push(10); st.push(20); st.push(30);
    REQUIRE(st.size() == 3);
    REQUIRE(st.top() == 30);
    st.pop();
    REQUIRE(st.top() == 20);
    st.pop(); st.pop();
    REQUIRE(st.empty());
    REQUIRE_THROWS_AS(st.pop(), std::runtime_error);
}

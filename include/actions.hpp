#pragma once
#include <string>

enum class ActionType { Checkout, Return };

struct Action {
    ActionType type;
    std::string isbn;
    int delta; // -1 for checkout, +1 for return
};

#pragma once
#include <string>
#include <cstdint>

struct Book {
    std::string isbn;     // unique key
    std::string title;
    std::string author;
    int32_t year{};
    int32_t copies_total{};
    int32_t copies_available{};

    bool operator<(const Book& other) const { return isbn < other.isbn; }
    bool operator==(const Book& other) const { return isbn == other.isbn; }
};

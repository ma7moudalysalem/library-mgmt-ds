#pragma once
#include <vector>
#include <optional>
#include <string>

struct BookRef { std::string isbn; int index; };

inline std::optional<int> binary_search_isbn(const std::vector<BookRef>& sorted, const std::string& key){
    int lo = 0, hi = static_cast<int>(sorted.size()) - 1;
    while(lo <= hi){
        int mid = lo + (hi - lo)/2;
        if(sorted[mid].isbn == key) return sorted[mid].index;
        if(sorted[mid].isbn < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return std::nullopt;
}

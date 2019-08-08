
#include "linecount.h"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace stiX {
    size_t linecount(std::istream &in) {
        return std::count_if(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                [](char c) { return c == '\n'; }
        );
    }
}

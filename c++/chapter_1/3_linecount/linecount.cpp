
#include "linecount.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace stiX {
    size_t linecount(std::istream &in) {
        return std::count(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                '\n'
        );
    }
}

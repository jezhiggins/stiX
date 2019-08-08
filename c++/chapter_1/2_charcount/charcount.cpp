#include "charcount.h"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace stiX {
    size_t charcount(std::istream &in) {
        return std::distance(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>()
        );
    }
}
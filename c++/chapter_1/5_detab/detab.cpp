#include <algorithm>
#include <iostream>

#include "detab.h"

namespace stiX {
    void detab(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostreambuf_iterator<char>(out),
                [](char c) { return c; }
        );
    }
}
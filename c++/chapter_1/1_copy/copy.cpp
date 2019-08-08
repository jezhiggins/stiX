
#include "copy.h"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace stiX {
    void copy(std::istream &in, std::ostream &out) {
        std::copy(
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(out)
        );
    }
}
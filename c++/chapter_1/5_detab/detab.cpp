#include <algorithm>
#include <iostream>

#include "detab.h"

struct detabber {
    char operator()(char c) {
        return c;
    }
};

namespace stiX {
    void detab(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostreambuf_iterator<char>(out),
                detabber()
        );
    }
}
#include <algorithm>
#include <iostream>

#include "detab.h"

struct detabber {
    std::string operator()(char c) {
        if (c == '\t')
            return std::string("        ");
        return std::string(1, c);
    }
};

namespace stiX {
    void detab(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostream_iterator<std::string>(out),
                detabber()
        );
    }
}
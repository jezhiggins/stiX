#include "entab.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

struct entabber {
    std::string operator()(char c) {
        return std::string(1, c);
    }
};

namespace stiX {
    void entab(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostream_iterator<std::string>(out),
                entabber()
        );
    }
}


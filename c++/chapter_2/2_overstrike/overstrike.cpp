#include "overstrike.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include "../../lib/chars.h"

struct overstriker {
    std::string const empty;
    std::string const skip = " ";
    std::string const noskip = "+";

    std::string output;
    std::string overstrike;
    size_t position_;
    std::string operator()(char c) {
        if (stiX::isbackspace(c)) {
            // do something with backspaces
            return empty;
        }

        output = (position_ == 0)
                ? skip
                : empty;
        output += c;

        if (stiX::isnewline(c))
            position_ = 0;
        else
            ++position_;

        return output;
    }
};

namespace stiX {
    void overstrike(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostream_iterator<std::string>(out),
                overstriker()
        );
    }
}


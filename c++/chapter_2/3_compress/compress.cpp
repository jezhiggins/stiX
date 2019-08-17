#include "compress.h"
#include "../../lib/filter.h"

struct compressor {
    std::string const countEncoding =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string const empty;

    char lastChar = 0;
    size_t repeat = 0;
    std::string output;

    std::string operator()(char c) {
        if (lastChar == c) {
            ++repeat;
            return empty;
        }

        output = empty;

        if (repeat) {
            if (repeat >= 3) {
                output += '~';
                output += countEncoding[repeat];
            } else {
                while (repeat-- > 0)
                    output += lastChar;
            }
            repeat = 0;
        }

        output += c;
        lastChar = c;

        return output;
    }
};

namespace stiX {
    void compress(std::istream &in, std::ostream &out) {
        filter(in, out, compressor());
    }
}


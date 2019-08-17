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

        if (repeat) {
            output = repeated();
            repeat = 0;
        } else {
            output = empty;
        }

        output += c;
        lastChar = c;

        return output;
    }

    std::string repeated() {
        if (repeat < 3)
            return std::string(repeat, lastChar);

        std::string r("~");
        r += countEncoding[repeat];
        return r;
    }
};

namespace stiX {
    void compress(std::istream &in, std::ostream &out) {
        filter(in, out, compressor());
    }
}


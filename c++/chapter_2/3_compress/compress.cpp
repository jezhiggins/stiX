#include "compress.h"
#include "../../lib/filter.h"

class compressor {
public:
    compressor(std::ostream& out)
        : out_(out) {
    }
    ~compressor() {
        if (repeat)
            out_ << repeated();
    }

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

private:
    std::string repeated() {
        if (repeat < 3)
            return std::string(repeat, lastChar);

        std::string r("~");
        r += countEncoding[repeat];
        return r;
    }

    std::string const countEncoding =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string const empty;

    std::ostream& out_;
    char lastChar = 0;
    size_t repeat = 0;
    std::string output;
};

namespace stiX {
    void compress(std::istream &in, std::ostream &out) {
        filter(in, out, compressor(out));
    }
}


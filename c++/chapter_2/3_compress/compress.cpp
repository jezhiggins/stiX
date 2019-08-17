#include "compress.h"
#include "../../lib/filter.h"

class compressor {
public:
    compressor(std::ostream& out)
        : out_(out) {
    }
    ~compressor() {
        if (repeat)
            out_ << repeated(repeat);
        else if (lastChar)
            out_ << lastChar;
    }

    std::string operator()(char c) {
        if (lastChar == c) {
            ++repeat;
            return empty;
        }

        if (repeat) {
            output = repeated(repeat);
            repeat = 0;
        } else {
            output.clear();
            if(lastChar)
                output += lastChar;
        }

        lastChar = c;

        return output;
    }

private:
    std::string repeated(size_t count) {
        if (count > 25) {
            return repeated(25)
              + repeated(count - 26);
        }
        if (count < 3)
            return std::string(count+1, lastChar);

        std::string r = marker;
        r += countEncoding[count];
        r += lastChar;
        return r;
    }

    std::string const countEncoding =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string const empty;
    std::string const marker = "~";

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


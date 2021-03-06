#include "compress.hpp"
#include "../../lib/filter.hpp"

class compressor {
public:
    compressor(std::ostream& out)
        : out_(out) {
    }
    ~compressor() {
       out_ << repeated(repeat);
    }

    std::string operator()(char c) {
        if (lastChar == c) {
            ++repeat;
            return empty;
        }

        std::string output = repeated(repeat);
        repeat = 0;

        lastChar = c;

        return output;
    }

private:
    std::string repeated(size_t count) {
        if (!lastChar)
            return empty;

        if (count > maxCount) {
            return repeated(maxCount)
              + repeated(count - (maxCount+1));
        }
        if ((count < 3) && (lastChar != marker))
            return std::string(count+1, lastChar);

        std::string r;
        r += marker;
        r += countEncoding[count];
        r += lastChar;
        return r;
    }

    std::string const countEncoding =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t const maxCount = countEncoding.size() - 1;
    std::string const empty;
    char const marker = '~';

    std::ostream& out_;
    char lastChar = 0;
    size_t repeat = 0;
};

namespace stiX {
    void compress(std::istream &in, std::ostream &out) {
        filter(in, out, compressor(out));
    }
}


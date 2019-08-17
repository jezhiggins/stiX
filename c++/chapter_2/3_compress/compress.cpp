#include "compress.h"
#include "../../lib/filter.h"

struct compressor {
    std::string operator()(char c) {
      return std::string(1, c);
    }
};

namespace stiX {
    void compress(std::istream &in, std::ostream &out) {
        filter(in, out, compressor());
    }
}


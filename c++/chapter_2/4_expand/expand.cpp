#include "expand.h"
#include "../../lib/filter.h"

enum ExpandState {
    passthrough,
    haveMarker,
    haveCount
};

class expander {
public: 
    std::string operator()(char c) {
        switch (state) {
            case haveMarker:
                state = haveCount;
                count = expandCount(c);
                return empty;
            case haveCount:
                state = passthrough;
                return std::string(count, c);
            case passthrough:
                if (c == '~') {
                    state = haveMarker;
                    return empty;
                }
        }
        return std::string(1, c);
    }

private:
    size_t expandCount(char c) {
        auto position = countEncoding.find(c);
        return position != std::string::npos
            ? position + 1
            : std::string::npos;
    }

    ExpandState state = passthrough;
    size_t count;

    std::string const countEncoding =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string const empty;
};

namespace stiX {
    void expand(std::istream& in, std::ostream& out) {
        filter(in, out, expander());
    }
}
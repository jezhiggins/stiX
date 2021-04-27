#include <optional>
#include "expand.hpp"
#include "../../lib/filter.hpp"

enum ExpandState {
    passthrough,
    haveMarker,
    haveCount
};

class expander {
public:
    expander(std::ostream& out)
      : out_(out) {
    }
    ~expander() {
      if (state == passthrough)
        return;

      out_ << recoveryString;
      if (state == haveCount)
        out_ << lastChar;
    }

    std::string operator()(char c) {
        lastChar = c;

        switch (state) {
            case passthrough:
                if (ismarker(c)) {
                    state = haveMarker;
                    return empty;
                }
                return std::string(1, c);
            case haveMarker:
                count = decodeCount(c);
                if (count == std::string::npos)
                    return recover(c);
                state = haveCount;
                return empty;
            case haveCount:
                state = passthrough;
                return std::string(count, c);
        }
    }

private:
    bool ismarker(char c) { return c == marker; }

    size_t decodeCount(char c) {
        auto position = countEncoding.find(c);
        return position != std::string::npos
            ? position + 1
            : std::string::npos;
    }

    std::string recover(char c) {
        if (ismarker(c)) // repeated marker
          return recoveryString;

        // bad count
        state = passthrough;
        return recoveryString + c;
    }

    std::ostream& out_;
    ExpandState state = passthrough;
    size_t count;
    char lastChar;

    std::string const countEncoding =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char const marker = '~';
    std::string const recoveryString = std::string(1, marker);
    std::string const empty;
};

namespace stiX {
    void expand(std::istream& in, std::ostream& out) {
        filter(in, out, expander(out));
    }
}
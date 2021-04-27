#include "overstrike.hpp"
#include "../../lib/filter.hpp"
#include "../../lib/chars.hpp"

class overstriker {
public:
    std::string operator()(char c) {
        if (stiX::isbackspace(c)) {
            ++backspaced_;
            return empty;
        }

        std::string output;

        if (backspaced_) {
            position_ = (backspaced_ < position_) ? position_ - backspaced_ : 0;
            output += noskip;
            output += std::string(position_, ' ');
        } else if (position_ == 0)
            output += skip;

        output += c;

        if (stiX::isnewline(c))
            position_ = 0;
        else
            ++position_;
        backspaced_ = 0;

        return output;
    }
private:
    std::string const empty;
    std::string const skip = " ";
    std::string const noskip = "\n+";

    size_t position_;
    size_t backspaced_;
};

namespace stiX {
    void overstrike(std::istream &in, std::ostream &out) {
        filter(in, out, overstriker());
    }
}


#include "entab.hpp"
#include "../../lib/chars.hpp"
#include "../../lib/tab_stops.hpp"
#include "../../lib/filter.hpp"

class entabber {
public:
    std::string operator()(char c) {
        if (stiX::isspace(c)) {
            ++spaces_;

            if (stiX::is_tab_stop(spaces_)) {
                position_ = spaces_;
                return tab;
            }

            return empty;
        }

        std::string output = (spaces_ != position_)
                ? std::string(spaces_ - position_, ' ')
                : empty;
        output += c;

        position_ = (stiX::isnewline(c))
            ? 0
            : position_ + 1;
        spaces_ = position_;

        return output;
    }
private:
    size_t position_;
    size_t spaces_;
    std::string const tab = "\t";
    std::string const empty;
};

namespace stiX {
    void entab(std::istream &in, std::ostream &out) {
        filter(in, out, entabber());
    }
}


#include "entab.h"
#include "../../lib/chars.h"
#include "../../lib/tab_stops.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

struct entabber {
    size_t position_;
    size_t spaces_;
    std::string const tab = "\t";
    std::string const empty;
    std::string output;

    std::string operator()(char c) {
        if (stiX::isspace(c)) {
            ++spaces_;

            if (stiX::is_tab_stop(spaces_)) {
                position_ = spaces_;
                return tab;
            }

            return empty;
        }

        output = (spaces_ != position_)
                ? std::string(spaces_ - position_, ' ')
                : empty;
        output += c;

        if (stiX::isnewline(c))
            position_ = 0;
        else
            ++position_;
        spaces_ = position_;

        return output;
    }
};

namespace stiX {
    void entab(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostream_iterator<std::string>(out),
                entabber()
        );
    }
}


#include "detab.hpp"
#include "../../lib/chars.hpp"
#include "../../lib/tab_stops.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

struct detabber {
    size_t position_;
    std::string operator()(char c) {
        if (stiX::istab(c)) {
            const auto spaces = stiX::distance_to_next_tab_stop(position_);
            position_ += spaces;
            return std::string(spaces, ' ');
        }

        if (stiX::isnewline(c))
            position_ = 0;
        else
            ++position_;

        return std::string(1, c);
    }
};

namespace stiX {
    void detab(std::istream &in, std::ostream &out) {
        std::transform(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>(),
                std::ostream_iterator<std::string>(out),
                detabber()
        );
    }
}
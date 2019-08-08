#include "detab.h"
#include "tab_stops.h"

#include <algorithm>
#include <iostream>
#include <iterator>

struct detabber {
    size_t position_;
    std::string operator()(char c) {
        if (c == '\t') {
            const auto spaces = stiX::distance_to_next_tab_stop(position_);
            position_ = stiX::next_tab_stop(position_);
            return std::string(spaces, ' ');
        }

        if (c == '\n')
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
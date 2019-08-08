#include "entab.h"
#include "../../lib/chars.h"
#include "../../lib/tab_stops.h"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <string>

struct entabber {
    size_t position_;
    size_t buffer_;

    std::string operator()(char c) {
        if (stiX::isspace(c)) {
            ++buffer_;

            if (stiX::is_tab_stop(buffer_)) {
                position_ = buffer_;
                return std::string("\t");
            }

            return std::string();
        }

        std::string output;
        if (buffer_ != position_) {
            output = std::string(buffer_ - position_, ' ');
        }
        output += c;

        if (stiX::isnewline(c))
            position_ = 0;
        else
            ++position_;
        buffer_ = position_;

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

